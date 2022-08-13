(*
   Shared.pas - Shared functionality

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

unit Shared;
{$inline on}

(* This unit contains shared functionality, especially constants,
   types and essential functions for multiple parts of the program *)

interface

uses Classes, Graphics;

type
  PTextFile = ^TextFile;
  TMapEntry = record
    offset : int64;
    length : int64;
    status : char;
  end;
  TMap = Array of TMapEntry;
  TRescueStatus = record
    devicesize : int64;
    suggestedBlockSize : integer;
    pos : int64;
    rescued : int64;
    nontried : int64;
    bad : int64;
    nonscraped : int64;
    nontrimmed : int64;
    outsidedomain : int64;
    errors : int64;
    curOperation : Char;
    strCurOperation : String; { TODO : make some use of this field }
    current_pass : integer;
  end;
  (* size display object *)
  TSFRounding = (rShortInt, rShortFloat, rSector, rNone);
  TSFBase = (bDec, bHex);
  TSFPrefix = (pDec, pBin);
  TSizeFormat = object
    Rounding : TSFRounding;
    Base : TSFBase;
    Prefix : TSFPrefix;
    SectSize : Longint;
    function SizeStr(size: Int64): String;
  end;
  (* Extend the IFPObserved intf to have Attach/detach wrappers
     that can be implemented as public methods by implementing classes.
     This would make these classes actually 'attachable' from the outside. *)
  IAttachableObserved = Interface(IFPObserved)
    procedure AttachObserver(AObserver : TObject);
    procedure DetachObserver(AObserver : TObject);
  end;

  TObservablePersistent = class(TPersistent, IAttachableObserved)
  (* Implement the IAttachableObserved interface for TPersistent.
     Predefined TPersistent descendants that need to be attachable should
     implement it like here *)
  public
    procedure AttachObserver(AObserver : TObject);
    procedure DetachObserver(AObserver : TObject);
  end;


const
  // block type masks for each block flag.
  // The upper 8 bits are reserved for these flags,
  // the lower 24 bits contain the color used for display.
  // The actual display color is blended together from the flags
  // in the upper 8 bits by ColorizeBlockMask
  MASK_OUTSIDE_DOMAIN = $01000000;
  MASK_NON_TRIED = $02000000;
  MASK_NON_TRIMMED = $04000000;
  MASK_NON_SCRAPED = $08000000;
  MASK_BAD_SECT = $10000000;
  MASK_FINISHED = $20000000;
  MASK_ALL_STATUSES = $3F000000;
  MASK_ACTIVE = $40000000;
  MASKS : array[0..5] of Longint =
    (MASK_OUTSIDE_DOMAIN, MASK_NON_TRIED, MASK_FINISHED, MASK_NON_TRIMMED,
     MASK_NON_SCRAPED, MASK_BAD_SECT);

  // color defaults
  DEF_COLOR_OUTSIDE_DOMAIN = $343434;
  DEF_COLOR_NON_TRIED = $909090;
  DEF_COLOR_NON_TRIMMED = $00e0ff;
  DEF_COLOR_NON_SCRAPED = $ff2020;
  DEF_COLOR_BAD_SECT = $0000ff;
  DEF_COLOR_FINISHED = $20e020;
  DEF_COLOR_UNDEFINED = $606060;
  DEF_COLOR_ACTIVE = $ffff00;
  DEF_COLOR_SELECTED = $ffffff;
  COLOR_HIGHLIGHT = clBlack;

  // color weight defaults to be used by ColorizeBlockMask for blending
  DEF_WEIGHT_OUTSIDE_DOMAIN = 1;
  DEF_WEIGHT_NON_TRIED = 3;
  DEF_WEIGHT_FINISHED = 5;
  DEF_WEIGHT_NON_TRIMMED = 12;
  DEF_WEIGHT_NON_SCRAPED = 20;
  DEF_WEIGHT_BAD_SECT = 80;

  // color primary masks
  MASK_B = $ff0000;
  MASK_G = $00ff00;
  MASK_R = $0000ff;

  // default device block size
  DEF_BSIZE = 512;

  PROGRAM_TITLE = 'ddrescueview';
  VERSION_MAJOR = '0';
  VERSION_MINOR = '4';
  VERSION_PATCH = '5';
  VERSION_SUFFIX = '';
  emptyRescueStatus : TRescueStatus =
  (devicesize : 0; suggestedBlockSize : DEF_BSIZE; pos : 0; rescued : 0;
   nontried : 0; bad : 0; nonscraped : 0; nontrimmed : 0; outsidedomain : 0;
   errors : 0; curOperation : #0; strCurOperation : ''; current_pass : 0);

var
  useDecimalUnits : boolean = true;
  // colors variables
  COLOR_OUTSIDE_DOMAIN : Longint = DEF_COLOR_OUTSIDE_DOMAIN;
  COLOR_NON_TRIED : Longint = DEF_COLOR_NON_TRIED;
  COLOR_NON_TRIMMED : Longint = DEF_COLOR_NON_TRIMMED;
  COLOR_NON_SCRAPED : Longint = DEF_COLOR_NON_SCRAPED;
  COLOR_BAD_SECT : Longint = DEF_COLOR_BAD_SECT;
  COLOR_FINISHED : Longint = DEF_COLOR_FINISHED;
  COLOR_UNDEFINED : Longint = DEF_COLOR_UNDEFINED;
  COLOR_ACTIVE : Longint = DEF_COLOR_ACTIVE;
  COLOR_SELECTED : Longint = DEF_COLOR_SELECTED;
  // color weights to be used by ColorizeBlockMask for blending
  WEIGHT_OUTSIDE_DOMAIN : Longint = DEF_WEIGHT_OUTSIDE_DOMAIN;
  WEIGHT_NON_TRIED : Longint = DEF_WEIGHT_NON_TRIED;
  WEIGHT_FINISHED : Longint = DEF_WEIGHT_FINISHED;
  WEIGHT_NON_TRIMMED : Longint = DEF_WEIGHT_NON_TRIMMED;
  WEIGHT_NON_SCRAPED : Longint = DEF_WEIGHT_NON_SCRAPED;
  WEIGHT_BAD_SECT : Longint = DEF_WEIGHT_BAD_SECT;
  // arrays pointing at the above values for easier looping
  COLORS : array[0..5] of ^Longint =
    (@COLOR_OUTSIDE_DOMAIN, @COLOR_NON_TRIED, @COLOR_FINISHED,
     @COLOR_NON_TRIMMED, @COLOR_NON_SCRAPED, @COLOR_BAD_SECT);
  WEIGHTS : array[0..5] of ^Longint =
    (@WEIGHT_OUTSIDE_DOMAIN, @WEIGHT_NON_TRIED, @WEIGHT_FINISHED,
     @WEIGHT_NON_TRIMMED, @WEIGHT_NON_SCRAPED, @WEIGHT_BAD_SECT);
  // Default size formatters
  SF_FLOAT : TSizeFormat = (Rounding : rShortFloat; Base : bDec; Prefix : pDec; SectSize : DEF_BSIZE);
  SF_SECT : TSizeFormat = (Rounding : rSector; Base : bDec; Prefix : pDec; SectSize : DEF_BSIZE);
  SF_HEX : TSizeFormat = (Rounding : rNone; Base : bHex; Prefix : pDec; SectSize : DEF_BSIZE);
  SFORMATS : array[0..2] of ^TSizeFormat = (@SF_FLOAT, @SF_SECT, @SF_HEX);

function BlockStatusToString(status: char) : String;
function OperationToText(status: char; pass: Integer) : string;
function BlockOverlap(b1Start, b1End, b2Start, b2End : Int64): Int64;
function verLT(versionA, versionB : String) : Boolean;
function blendColors(color1, color2: TColor; intensity1 : integer): TColor; inline;
function InRangeEx(const AValue, A1, A2: Longint): Boolean; inline;
function IntersectEntries(entry1, entry2 : TMapEntry): TMapEntry;
function FilePart(path: String): String;
function DateTimeToMilliseconds(aDateTime: TDateTime): Int64;

implementation
uses SysUtils, Math;

// status strings for each block status
function BlockStatusToString(status: char) : String;
begin
  case status of
    '?': result := 'Non-tried';
    '+': result := 'Rescued';
    '*': result := 'Non-trimmed';
    '/': result := 'Non-scraped';
    '-': result := 'Bad sector(s)';
    'd': result := 'Not in domain';
  else
    result := 'Unknown';
    // if status is the "empty char" (ordinal 0) it must not be added to the string.
    // --> check it is a printable char (higher than space in ascii)
    if status >= ' ' then result += ' ('+status+')';
  end;
end;

// status strings for the status line
function OperationToText(status: char; pass: Integer) : string;
begin
  case status of
    '?': result := 'Copying non-tried blocks';
    '*': result := 'Trimming non-trimmed blocks';
    '/': result := 'Scraping non-scraped blocks';
    '-': result := 'Retrying bad sectors';
    'F': result := 'Filling specified blocks';
    'G': result := 'Generating approximate mapfile';
    '+': result := 'Finished';
  else
    result := 'Unknown';
    if status >= ' ' then result += ' ('+status+')';
  end;
  if pass > 0 then result += ' (Pass '+IntToStr(pass)+')';
end;

function TSizeFormat.SizeStr(size: Int64): String;
const
  decPrefixes : array[0..5] of String = ('Byte', 'KB', 'MB', 'GB', 'TB', 'PB');
  binPrefixes : array[0..5] of String = ('Byte', 'KiB', 'MiB', 'GiB', 'TiB', 'PiB');
var
  sPrefix: string;
  iSectSize, thresh, divisor : Longint;
  i : integer;
  iValue : Int64;
  fValue : double;
begin
  if Base = bHex then begin
    result:='0x'+IntToHex(size, 8);
    exit;
  end;
  if Rounding = rSector then begin
    if SectSize < 1 then iSectSize:=512 else iSectSize:=SectSize;
    result:=IntToStr(size div iSectSize)+' s';
    exit;
  end;
  if Rounding = rNone then begin
    result:=IntToStr(size)+' Byte';
    exit;
  end;
  if Prefix = pDec then divisor:=1000 else divisor:=1024;
  if Rounding = rShortInt then thresh:=divisor*100 else thresh:=divisor;
  iValue:=size;
  fValue:=size;
  i:=0;
  while i < High(decPrefixes) do begin
    if Rounding = rShortInt then begin
      if iValue < thresh then break;
      iValue := iValue div divisor;
      inc(i);
    end else begin
      if fValue < thresh then break;
      fValue := fValue / divisor;
      inc(i);
    end;
  end;
  if Prefix = pDec then sPrefix:=decPrefixes[i] else sPrefix:=binPrefixes[i];
  if (i = 0) or (Rounding = rShortInt) then result:=IntToStr(iValue)+' '+sPrefix
  else result:=FloatToStrF(fValue, ffFixed, 12, 2)+' '+sPrefix;
end;

// returns the number of bytes by which two blocks on a device overlap.
function BlockOverlap(b1Start, b1End, b2Start, b2End: Int64): Int64;
begin
  BlockOverlap:=Max(0, Min(Min(b1End-b1Start, b2End-b2Start),
                           Min(b2End-b1Start, b1End-b2Start)));
end;

// return the intersection of two map entries. Status is copied from first.
function IntersectEntries(entry1, entry2 : TMapEntry): TMapEntry;
begin
  result.status:=entry1.status;
  result.length:=BlockOverlap(entry1.offset, entry1.offset+entry1.length,
                              entry2.offset, entry2.offset+entry2.length);
  result.offset:=max(entry1.offset, entry2.offset);
end;

// returns the file part of a file path
function FilePart(path: String): String;
var delimPos : Integer;
begin
  delimPos := LastDelimiter('\/:', path);
  result := Copy(path, delimPos+1, Length(path)-delimPos);
end;

// compare two version strings and return true if versionA is less than versionB
// could be used with TParser.Version in the future to adapt to ddrescue versions
function verLT(versionA, versionB : String) : Boolean;
var
  listA, listB : TStringList;
  intsA, intsB : array of integer;
  i, j : integer;
begin
  // set up the string lists
  listA:=TStringList.Create;
  listB:=TStringList.Create;
  listA.StrictDelimiter:=true;
  listB.StrictDelimiter:=true;
  listA.Delimiter:='.';
  listB.Delimiter:='.';
  // read in the delimited text
  listA.DelimitedText:=versionA;
  listB.DelimitedText:=versionB;
  // Extend both lists to the same length by appending zeroes
  while listA.Count < listB.Count do listA.Add('0');
  while listB.Count < listA.Count do listB.Add('0');
  // strip non-numeric suffixes from the version parts
  for i := 0 to listA.Count-1 do begin
    for j := 1 to Length(listA[i]) do begin
      if not (listA[i][j] in ['0'..'9']) then begin
        listA[i]:=Copy(listA[i], 1, j-1);
        break;
      end;
    end;
    for j := 1 to Length(listB[i]) do begin
      if not (listB[i][j] in ['0'..'9']) then begin
        listB[i]:=Copy(listB[i], 1, j-1);
        break;
      end;
    end;
  end;
  // set up the integer lists.
  SetLength(intsA, listA.Count);
  SetLength(intsB, listB.Count);
  // Convert the string parts of each version to an int list
  for i := 0 to listA.Count-1 do begin
    intsA[i]:=StrToIntDef(listA[i], 0);
    intsB[i]:=StrToIntDef(listB[i], 0);
  end;
  // free the lists
  listA.Free;
  listB.Free;
  // Compare the list elements
  for i := 0 to Length(intsA)-1 do begin
    if intsA[i] < intsB[i] then exit(true)
    else if intsB[i] < intsA[i] then exit(false);
  end;
  // at this point the versions are equal
  verLT:=false;
end;

// Blends two colors. intensity1 [0..256] determines the strength of color1.
// intensity1 values over 256 will cause color corruption
// this is more optimized for speed than for clarity or rounding accuracy.
function blendColors(color1, color2: TColor; intensity1 : integer): TColor; inline;
const mask = $FF00FF00FF;
var tmp: Int64;
begin
  tmp:=(((Int64(color1)*$1000001 and mask)*intensity1 +
         (Int64(color2)*$1000001 and mask)*(256-intensity1)) shr 8) and mask;
  result:= TColor(tmp) or TColor(tmp shr 24);
end;

// allows Min/Max args to be reversed
function InRangeEx(const AValue, A1, A2: Longint): Boolean; inline;
begin
  Result:=((AValue>=A1) and (AValue<=A2)) or ((AValue>=A2) and (AValue<=A1));
end;

procedure TObservablePersistent.AttachObserver(AObserver : TObject);
begin
  FPOAttachObserver(AObserver);
end;

procedure TObservablePersistent.DetachObserver(AObserver : TObject);
begin
  FPODetachObserver(AObserver);
end;

function DateTimeToMilliseconds(aDateTime: TDateTime): Int64;
var
  TimeStamp: TTimeStamp;
begin
  {Call DateTimeToTimeStamp to convert DateTime to TimeStamp:}
  TimeStamp:= DateTimeToTimeStamp (aDateTime);
  {Multiply and add to complete the conversion:}
  Result:= TimeStamp.Time;
end;

end.
