(*
   Parser.pas - Mapfile parser unit

   Copyright (C) 2013 - 2022 Martin Bittermann (martinbittermann@gmx.de)

   This file is part of ddrescueview.

   ddrescueview is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ddrescueview is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with ddrescueview.  If not, see <http://www.gnu.org/licenses/>.
*)

// {$DEFINE VerboseProfiler}

unit Parser;

interface
uses Classes, Shared;

type

  { TSimpleParser }

  TSimpleParser = class(TObservablePersistent)
  private
    FMap : TMap;
    FRescueStatus : TRescueStatus;
    FComments : TStringList; // comment lines of mapfile
    FVersion : String; // ddrescue version in first comment line / for future use?
    FFileName : String;
    procedure logMsg(msg : String);
    function isEmptyLine(strLine : String) : boolean;
    function isCommentLine(strLine : String) : boolean;
  public
    constructor Create;
    destructor Destroy; override;
    procedure OpenFile(filename : String);
    procedure CloseFile();
    function parse(forceSameDevSize : Boolean) : Boolean;
    function hasFile() : boolean;
    property rescueStatus : TRescueStatus read FRescueStatus;
    property map : TMap read FMap;
    property CommentLines : TStringList read FComments;
    property Version : String read FVersion;
    property MapFileName : String read FFileName;
  end;


  { TMapParser }

  // This parser can also embed a domain mapfile parser
  TMapParser = class(TObservablePersistent)
  private
    FMapParser : TSimpleParser;
    FDomParser : TSimpleParser;
    FMap: TMap;
    FRescueStatus : TRescueStatus;
    FContiguous : Boolean;
    FHasFile : Boolean;
    function getMap(): TMap;
    procedure postParse();
    procedure setContiguous(mode: Boolean);
    function getCommentLines(): TStringList;
    function getVersion(): String;
    function getMapFileName(): String;
    function getDomFileName(): String;
  public
    constructor Create;
    destructor Destroy; override;
    procedure OpenFile(filename : String);
    procedure OpenDomainFile(filename : String);
    procedure CloseFile();
    function parse(forceSameDevSize : Boolean) : Boolean;
    function hasFile() : boolean;
    function hasDomFile() : Boolean;
    property rescueStatus : TRescueStatus read FRescueStatus;
    property map : TMap read getMap;
    property CommentLines : TStringList read GetCommentLines;
    property Version : String read getVersion;
    property MapFileName : String read getMapFileName;
    property DomFileName : String read getDomFileName;
    property ContiguousDomain : Boolean read FContiguous write setContiguous;
  end;

implementation
uses SysUtils, Math, GUI;

{ TMapParser }

constructor TMapParser.Create;
begin
  inherited;
  FRescueStatus := emptyRescueStatus;
  FMapParser:=TSimpleParser.Create;
  FDomParser:=TSimpleParser.Create;
end;

destructor TMapParser.Destroy;
begin
  CloseFile();
  FreeAndNil(FDomParser);
  FreeAndNil(FMapParser);
  inherited Destroy;
end;

procedure TMapParser.OpenFile(filename: String);
begin
  CloseFile;
  FMapParser.OpenFile(filename);
  postParse;
end;

procedure TMapParser.OpenDomainFile(filename: String);
begin
  FDomParser.OpenFile(filename);
  if FDomParser.hasFile and FContiguous then begin
    FHasFile:=false;
    FPONotifyObservers(self, ooDeleteItem, nil);
  end;
  postParse;
end;

procedure TMapParser.CloseFile();
begin
  FDomParser.CloseFile;
  FMapParser.CloseFile;
  FRescueStatus := emptyRescueStatus;
  SetLength(FMap, 0);
  FHasFile:=false;
  FPONotifyObservers(self, ooDeleteItem, nil);
end;

procedure TMapParser.postParse();
var
  ResMap, DomMap: TMap;
  i, iRes, iDom : Longint;
  newEntry : TMapEntry;
  mapCurOffset : Int64;
begin
  if FDomParser.hasFile then begin // post-process the maps
    FRescueStatus:=emptyRescueStatus;
    ResMap:=FMapParser.map;
    DomMap:=FDomParser.map;
    //build map and rescuestatus
    i:=0; iRes:=0; iDom:=0; // current map indices
    mapCurOffset:=0; // needed for contiguous mode, but also used for normal mode
    while iDom < Length(DomMap) do begin
      if (DomMap[iDom].status<>'+') then begin // outside domain entry
        if not FContiguous then begin
          newEntry.status:='d';
          newEntry.offset:=DomMap[iDom].offset;
          newEntry.length:=DomMap[iDom].length;
          inc(FRescueStatus.outsidedomain, DomMap[iDom].length);
          mapCurOffset:=newEntry.offset+newEntry.length;
          if InRange(FMapParser.rescueStatus.pos, newEntry.offset, mapCurOffset-1) then
             FRescueStatus.pos:=FMapParser.rescueStatus.pos;
          if Length(FMap) <= i then SetLength(FMap, i+1024);
          FMap[i]:=newEntry;
          Inc(i);
        end;
      end else begin // iterate over the ResEntries intersecting current DomEntry
        while iRes < Length(ResMap) do begin
          // increase iRes until intersecting dom block
          if ResMap[iRes].offset+ResMap[iRes].length <= DomMap[iDom].offset then begin
            Inc(iRes);
            Continue;
          end;
          // if intersecting current dom block, create new block
          newEntry:=IntersectEntries(ResMap[iRes], DomMap[iDom]);
          if newEntry.length > 0 then begin //can be 0 for rescue mapfiles with gaps
            if InRange(FMapParser.rescueStatus.pos, newEntry.offset,
                       newEntry.offset+newEntry.length-1) then
              FRescueStatus.pos:=mapCurOffset+FMapParser.rescueStatus.pos-newEntry.offset;
            newEntry.offset:=mapCurOffset;
            mapCurOffset+=newEntry.length;
            if Length(FMap) <= i then SetLength(FMap, i+1024);
            FMap[i]:=newEntry;
            Inc(i);
            case newEntry.status of
              '?' : inc(FRescueStatus.nontried, newEntry.length);
              '+' : inc(FRescueStatus.rescued, newEntry.length);
              '*' : inc(FRescueStatus.nontrimmed, newEntry.length);
              '/' : inc(FRescueStatus.nonscraped, newEntry.length);
              '-' : begin
                      inc(FRescueStatus.bad, newEntry.length);
                      inc(FRescueStatus.errors);
                    end;
            end;
          end;
          // ensure iRes is set to last intersecting entry
          if ResMap[iRes].offset+ResMap[iRes].length >=
             DomMap[iDom].offset+DomMap[iDom].length then break;
          Inc(iRes);
        end;
      end;
      Inc(iDom);
    end;
    SetLength(FMap, i);
    FRescueStatus.curOperation:=FMapParser.rescueStatus.curOperation;
    FRescueStatus.strCurOperation:=FMapParser.rescueStatus.strCurOperation;
    if i > 0 then begin  // calculate device's size
      FRescueStatus.devicesize:=FMap[i-1].offset + FMap[i-1].length;
    end;
    FRescueStatus.suggestedBlockSize:=FMapParser.rescueStatus.suggestedBlockSize;
  end else begin
    FRescueStatus:=FMapParser.rescueStatus;
  end;
  if (not FHasFile) and FMapParser.hasFile then begin  // just been opened
    FHasFile:=true;
    FPONotifyObservers(self, ooAddItem, nil);
  end else FPONotifyObservers(self, ooChange, nil)
end;

procedure TMapParser.setContiguous(mode: Boolean);
begin
  if mode <> FContiguous then begin
    FContiguous:=mode;
    if FDomParser.hasFile then begin
      FHasFile:=false;
      FPONotifyObservers(self, ooDeleteItem, nil);
      postParse;
    end;
  end;
end;

function TMapParser.parse(forceSameDevSize : Boolean) : Boolean;
begin
  result := True;
  if FMapParser.hasFile() then result := FMapParser.parse(forceSameDevSize);
  if FDomParser.hasFile() then FDomParser.parse(false);
  postParse;
end;

function TMapParser.hasFile(): boolean;
begin
  result:=FHasFile;
end;

function TMapParser.hasDomFile(): Boolean;
begin
  result:=FHasFile and FDomParser.hasFile;
end;

function TMapParser.getMap(): TMap;
begin
  if not FDomParser.hasFile then result:=FMapParser.map
  else result:=FMap;
end;

function TMapParser.getCommentLines(): TStringList;
begin
  getCommentLines:=FMapParser.FComments;
end;

function TMapParser.getVersion(): String;
begin
  getVersion:=FMapParser.FVersion;
end;

function TMapParser.getMapFileName(): String;
begin
  getMapFileName:=FMapParser.FFileName;
end;

function TMapParser.getDomFileName(): String;
begin
  getDomFileName:=FDomParser.FFileName;
end;

{ TSimpleParser }

constructor TSimpleParser.Create;
begin
  inherited;
  FRescueStatus := emptyRescueStatus;
end;

destructor TSimpleParser.Destroy;
begin
  CloseFile();
  inherited;
end;

procedure TSimpleParser.OpenFile(filename : String);
var mapStream : TFileStream;
begin
  try
    if hasFile() then CloseFile(); // close already open file
    // check mapfile access once, and close the file again.
    mapStream := TFileStream.Create(filename, fmOpenRead or fmShareDenyNone);
    FreeAndNil(mapStream);
    // file checked, it can be considered 'open' now.
    FComments := TStringList.Create;
    FFileName := filename;
    parse(false);  // start parsing after opening
  except
    on E: Exception do begin
        CloseFile;
        logMsg('Parser error: '+E.Message);
    end;
  end;
end;

procedure TSimpleParser.CloseFile;
begin
  FFileName := '';
  FreeAndNil(FComments);
  FRescueStatus := emptyRescueStatus;
  SetLength(FMap, 0);
  FPONotifyObservers(self, ooDeleteItem, nil);
end;

(* returns true if a line is empty, assuming the line is already trimmed *)
function TSimpleParser.isEmptyLine(strLine : String) : boolean;
begin
  result := (Length(strLine) = 0); // empty line
end;

(* returns true if a line is a comment line, assuming the line is already trimmed *)
function TSimpleParser.isCommentLine(strLine : String) : boolean;
begin
  result := (Length(strLine) > 0) and (strLine[1] = '#'); // comment line
end;

(* add a log line to the application log *)
procedure TSimpleParser.logMsg(msg : String);
begin
  MainForm.DbgLog.Lines.Add(msg);
end;

(* Parse the mapfile.
   After parsing, the results are available in
   these properties: map, rescueStatus and Comments *)
function TSimpleParser.parse(forceSameDevSize : Boolean) : Boolean; //success
var
  line : string;
  token : array[0..2] of string;
  i, mapEntry, lineIdx, idx : Integer;
  mapStream : TFileStream = nil;
  mapStrings : TStringList = nil;
  prevHadFile, statuslineFound : boolean;
  newMap : TMap;
  newRescueStatus : TRescueStatus;
  newComments : TStringList;
  newVersion : String;
  {$IFDEF VerboseProfiler}
  lTimeStart: TDateTime;
  {$ENDIF}
begin
  result:=False;
  // make sure the parser has a file
  if not hasFile() then begin
    logMsg('Parser: No mapfile opened.');
    FPONotifyObservers(self, ooChange, nil);
    exit;
  end;
  {$IFDEF VerboseProfiler}
  lTimeStart := Now();
  {$ENDIF}
  // open the file and read its contents into string list
  try
    try
      mapStream := TFileStream.Create(FFilename, fmOpenRead or fmShareDenyNone);
      if mapStream.Seek(0, soFromBeginning) <> 0 then
        raise Exception.Create('Seek error!');
      mapStrings := TStringList.Create;
      mapStrings.LoadFromStream(mapStream);
      if mapStrings.Count = 0 then
        raise Exception.Create('File is empty!');
      logMsg('Reading mapfile: ' +IntToStr(mapStrings.Count) + ' lines.');
    except
      on E : Exception do begin
        logMsg('Error: Cannot read mapfile: '+E.Message+'('+E.ClassName+')');
        FreeAndNil(mapStrings);
        FPONotifyObservers(self, ooChange, nil);
        exit;
      end;
    end;
  finally
    FreeAndNil(mapStream); // close the map file immediately after reading
  end;

  {$IFDEF VerboseProfiler}
  logMsg(Format('[TSimpleParser.parse] File load Duration: %d ms', [DateTimeToMilliseconds(Now() - lTimeStart)]));
  lTimeStart := Now();
  {$ENDIF}

  // reserve space in the map. Less entries will be actually needed, due
  // to comment lines and the status line being counted.
  SetLength(newMap, mapStrings.Count);
  // initialize many things to their initial values
  newRescueStatus := emptyRescueStatus;
  newComments := TStringList.Create;
  newVersion:='';
  lineIdx := 0;
  mapEntry := 0;
  prevHadFile := Length(FMap) > 0;
  statuslineFound := false;

  // parse the mapfile lines into map array, rescueStatus and Comments.
  try
    while lineIdx < mapStrings.Count do begin
      line:=Trim(mapStrings[lineIdx]);
      if isCommentLine(line) then begin
        if not statuslineFound then begin //process comments only until the status line is found
          // copy comment line into FComments (but not the #currentpos one...)
          if pos('# current_pos', line) = 0 then newComments.Add(line);
          if pos('Command line:', line) > 0 then begin // try to find ddrescue's block size argument
             repeat // not a loop, but goto replacement
               idx := pos(' -b', line);
               if idx <> 0 then idx := idx+3 // point to start of number
               else begin
                 idx := pos(' --block-size=', line);
                 if idx <> 0 then idx := idx+14 // point to start of number
                 else break; // argument not found, jump over the rest
               end;
               token[0] :=TrimLeft(Copy(line, idx, Length(line)));
               idx := pos(' ', token[0]); //space after argument
               if idx <> 0 then token[0] :=Copy(token[0], 1, idx-1);
               newRescueStatus.suggestedBlockSize:=StrToIntDef(token[0], DEF_BSIZE);
             until true;
          end;
          idx:=pos('ddrescue version ', line);  // get ddrescue version
          if idx > 0 then newVersion := Copy(line, idx+17, 1337);
        end;
      end else if Length(line) > 0 then begin
        // split line into maximum of 3 tokens
        for i:= 0 to 2 do begin
          idx:= Pos(' ', line); //get pos of next delimiter
          if idx = 0 then begin //if no more delimiter...
            token[i] := line; //...the whole remainder of line is ours!
            line := '';
          end else begin   // otherwise our token = line until the pos of delimiter
            token[i] := Copy(line, 1, idx-1);
            line := TrimLeft(Copy(line, idx, Length(line)-idx+1));
          end;
        end;
        if (not statuslineFound) and (token[0] <> '') and (token[1] <> '') then begin
          // found the status line
          newRescueStatus.pos:=StrToInt64Def(token[0], 0);
          newRescueStatus.curOperation:=token[1][1];
          if token[2] <> '' then newRescueStatus.current_pass:=StrToIntDef(token[2], 0);
          newRescueStatus.strCurOperation:=OperationToText(newRescueStatus.curOperation, newRescueStatus.current_pass);
          statuslineFound := true;
        end else if (token[0] <> '') and (token[1] <> '') and (token[2] <> '') then begin
          // standard block line
          newMap[mapEntry].offset:=StrToInt64(token[0]);
          newMap[mapEntry].length:=StrToInt64(token[1]);
          newMap[mapEntry].status:=token[2][1];
          case newMap[mapEntry].status of
            '?' : inc(newRescueStatus.nontried, newMap[mapEntry].length);
            '+' : inc(newRescueStatus.rescued, newMap[mapEntry].length);
            '*' : inc(newRescueStatus.nontrimmed, newMap[mapEntry].length);
            '/' : inc(newRescueStatus.nonscraped, newMap[mapEntry].length);
            '-' : begin
                    inc(newRescueStatus.bad, newMap[mapEntry].length);
                    inc(newRescueStatus.errors);
                  end;
          end;
          inc(mapEntry);
        end else begin // unknown type or formatting of line
          logMsg('Parser: Cannot parse line '+inttostr(lineIdx));
        end;
      end;
      lineIdx+=1;
    end;
    SetLength(newMap, mapEntry); // trim array to actually needed size
  except
    on E : Exception do begin
      logMsg('Error parsing mapfile on line '+IntToStr(lineIdx+1)+': '+
             E.Message+'('+E.ClassName+')');
      if not forceSameDevSize then begin
        // Set some fail-safe values only if Same Device Size parsing is not forced
        newRescueStatus := emptyRescueStatus;
        newRescueStatus.strCurOperation:='PARSER ERROR';
        SetLength(newMap, 1);
        newMap[0].offset:=0;
        newMap[0].length:=DEF_BSIZE*(1 shl 18); // one block of 2^18 bad sectors
        newMap[0].status:='-';
        newRescueStatus.bad:=newMap[0].length;
        newRescueStatus.devicesize:=newMap[0].length;
      end;
    end;
  end;
  FreeAndNil(mapStrings);

  {$IFDEF VerboseProfiler}
  logMsg(Format('[TSimpleParser.parse] Parse Duration: %d ms', [DateTimeToMilliseconds(Now() - lTimeStart)]));
  {$ENDIF}

  // calculate device's size from last block's offset and length
  if Length(newMap) > 0 then begin
    newRescueStatus.devicesize:=newMap[Length(newMap)-1].offset + newMap[Length(newMap)-1].length;
  end else begin
    logMsg('Parser: No blocks in mapfile!');
    newRescueStatus.devicesize := 0;
  end;


  if (newRescueStatus.devicesize <> FRescueStatus.devicesize) and forceSameDevSize then begin
    // Do NOT swap the new data set in, since it has an invalid device size
    // and it's specified that we don't want that. Just deallocate memory we used.
    newComments.Free;
  end else begin
    // swap new data set in
    FRescueStatus := newRescueStatus;
    FMap := newMap;
    FComments.Free;
    FComments := newComments;
    FVersion := newVersion;
    result := true;
  end;

  if prevHadFile then FPONotifyObservers(self, ooChange, nil) // notify observers of the changes
     else FPONotifyObservers(self, ooAddItem, nil); // notify of new file
end;

function TSimpleParser.hasFile(): boolean;
begin
  hasFile:=Length(FFileName) > 0;
end;

end.
