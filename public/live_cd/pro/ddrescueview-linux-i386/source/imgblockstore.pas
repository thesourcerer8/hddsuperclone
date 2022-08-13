(*
   ImgBlockStore.pas - Image Block Storage unit

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

unit ImgBlockStore;

interface

uses
  Classes, SysUtils, Parser, Shared;

type

  TZoom = record
    SectOffset : int64; // imgBlocks start after this # of device sectors (zooming)
    SectCount : int64;  // number of device sectors to show in imgBlocks (zooming)
  end;
  { TImgBlockStore }
  TImgBlockStore = class(TObservablePersistent, IFPObserver)
  private
    FImgBlocks: Array of Longword; // lower 24 bits : color, upper 8 bits: status mask
    FParser : TMapParser; // parser / map source to use
    FNumBlocks : Longint;  // number of blocks requested
    FSectSize : Int64; // device sector size
    FZoom : TZoom;
    FSectInBlock : Int64; // device sectors per imgBlock
    FByteInBlock : Int64; // # of bytes in every imgBlock, multiple of FDevBSize
    FOwner : TObject;
    FNoZoom : Boolean;    // prevent zooming. Always show full device
    FLockOnCurPos : Boolean; // center around current position if possible but allow zoom
    FBlockstatusMasks : array[0..255] of Longword; // LUT, maps log entry statuses to Block Status masks
    procedure assignParser(source : TMapParser);
    procedure buildBlocks;
    function BlockStatusToMask(status: char) : Longword;
    function ColorizeBlockMask(mask: Longint) : Longint;
    function getBlock(i: Longint): Longword;
    procedure sanitizeZoom();
    function binSearchStartEntry() : integer;
    function binSearchEndEntry() : integer;
    procedure setDevBSize(size : Int64);
    procedure SetLockOnCurPos(lock: Boolean);
    procedure setNumBlocks(num : Longint);
    procedure setSectOffset(offset : Int64);
    procedure setSectCount(count : Int64);
  public
    constructor Create(AOwner : TObject); virtual;
    destructor Destroy; override;
    property Blocks[i:Longint] : Longword read getBlock; default;
    property Parser : TMapParser read FParser write assignParser;
    property NumBlocks : Longint read FNumBlocks write setNumBlocks;
    property SectSize : Int64 read FSectSize write setDevBSize;
    property SectOffset : Int64 read FZoom.SectOffset write setSectOffset;
    property SectCount : Int64 read FZoom.SectCount write setSectCount;
    property ByteInBlock : Int64 read FByteInBlock;
    property SectInBlock : Int64 read FSectInBlock;
    property NoZoom : Boolean read FNoZoom write FNoZoom;
    property LockOnCurPos : Boolean read FLockOnCurPos write SetLockOnCurPos;
    function getOffsetFromBlock(block : Int64) : Int64;
    function getBlockFromOffset(offset : Int64) : Int64;
    function activeBlock() : Int64;
    procedure setZoom(Offset, Count: Int64);
    procedure zoomToCenter(block: Longint; factor: double);
    procedure zoomBlockToBlock(srcBlock, destBlock: Longint; factor: double);
    procedure zoomInRange(block1, block2: Longint);
    Procedure FPOObservedChanged(ASender : TObject; //for the Observer role
      Operation : TFPObservedOperation; Data : Pointer);
  end;

implementation
uses GUI, Math;

// creates the ImgBlockStore and sets up listening for the owner.
constructor TImgBlockStore.Create(AOwner : TObject);
var c : byte;
begin
  inherited Create;
  FSectSize := DEF_BSIZE;
  FByteInBlock := DEF_BSIZE;
  FSectInBlock := 1;
  FOwner := AOwner;
  // Precompute the Block Status Masks LUT
  for c:= 0 to 255 do FBlockstatusMasks[c]:=BlockStatusToMask(char(c));
  // listen for requests from the owner
  if (AOwner<>nil) and (AOwner is IAttachableObserved) then
    (AOwner as IAttachableObserved).AttachObserver(self);
end;

destructor TImgBlockStore.Destroy;
begin
  SetLength(FImgBlocks, 0);
  if FParser<>nil then FParser.DetachObserver(self);
  if (FOwner<>nil) and (FOwner is IAttachableObserved) then
    (FOwner as IAttachableObserved).DetachObserver(self);
  inherited;
end;

procedure TImgBlockStore.FPOObservedChanged(ASender: TObject;
  Operation: TFPObservedOperation; Data: Pointer);
begin
  if ASender = FParser then begin // parser did its thing ...
    if Operation in [ooAddItem, ooDeleteItem] then begin // new or closed file!
      if Operation = ooAddItem then FSectSize:=FParser.rescueStatus.suggestedBlockSize;
      FZoom.SectCount:=0; // reset zoom
    end;
  end;
  sanitizeZoom();
  buildBlocks;
  // log the update to mainform textbox
  //MainForm.DbgLog.Lines.Add('ImgBlockStore update: ' + BoolToStr(FNoZoom, 'Zoom', 'Grid'));
  FPONotifyObservers(self, ooChange, nil);
end;

// Assign a MapParser to the img Blocks.
// Assign another parser before freeing the old parser from the outside!
procedure TImgBlockStore.assignParser(source : TMapParser);
begin
  if source=nil then raise Exception.Create('imgBlockStore: Cannot assign nil parser!');
  if FParser<>nil then FParser.DetachObserver(self); // detach old source
  FParser:=source;
  FSectSize:=FParser.rescueStatus.suggestedBlockSize;
  FZoom.SectCount:=0; // reset zoom
  FParser.AttachObserver(self);
  buildBlocks;
  FPONotifyObservers(self, ooChange, nil);
end;

function TImgBlockStore.getBlock(i: Longint): Longword;
begin
  getBlock:=FImgBlocks[i];
end;

// Gets the Offset of a given imgBlock on the rescue device, in bytes.
function TImgBlockStore.getOffsetFromBlock(block: Int64): Int64;
begin
  getOffsetFromBlock:=FZoom.SectOffset*FSectSize + FByteInBlock*block;
end;

// Gets the block index that belongs to a given offset on the device.
// Aside from round, this is the opposite of getOffsetFromBlock.
// Can be negative or greater than numBlocks-1 when zoomed.
function TImgBlockStore.getBlockFromOffset(offset : Int64) : Int64;
var localOffset : Int64;
begin
  localOffset := offset - FZoom.SectOffset*FSectSize;
  if FByteInBlock > 0 then result:=(localOffset div FByteInBlock) -
    Int64((localOffset mod FByteInBlock <> 0) and (localOffset < 0))
  else result:=0;
end;

// returns imgblock index of current position. May be out of range, see above.
function TImgBlockStore.activeBlock() : Int64;
begin
  if Assigned(FParser) then
    result:=getBlockFromOffset(FParser.rescueStatus.pos)
  else result:=0;
end;

// clean up the zoom settings:
// * set zoom to span whole device if not already set, or FNoZoom is true
// * limit zoom section to device size
// * recalculate FSectInBlock and FByteInBlock
procedure TImgBlockStore.sanitizeZoom();
var
  deviceSectors: Int64;
begin
  if (not Assigned(FParser)) or (FNumBlocks = 0) or (FSectSize = 0) then exit;
  with FParser do begin
    deviceSectors:=rescueStatus.devicesize div FSectSize;
    // first, if FLockOnCurPos is true, keep the current position locked in the center:
    if FLockOnCurPos and not FNoZoom then
      FZoom.SectOffset:=(rescueStatus.pos div FSectSize)-(FZoom.SectCount div 2);

    if (FZoom.SectCount=0) or FNoZoom then begin
      FZoom.SectOffset:=0;
      FZoom.SectCount:=deviceSectors;
    end;

    FSectInBlock := FZoom.SectCount div FNumBlocks + Int64((FZoom.SectCount mod FNumBlocks)<>0);
    FByteInBlock := FSectInBlock * FSectSize;
    FZoom.SectCount := Max(0, Min(FSectInBlock * FNumBlocks, deviceSectors - FZoom.SectOffset));
    // now if the user zoomed in all the way to the end of the device, some
    // (or many!) blocks at the end may be unused -> fix this by decrementing offset:
    FZoom.SectOffset -= min(FZoom.SectOffset, max(0,
      FSectInBlock * FNumBlocks - deviceSectors + FZoom.SectOffset));
    // now try to set larger sectCount again...
    FZoom.SectCount := Max(0, Min(FSectInBlock * FNumBlocks, deviceSectors - FZoom.SectOffset));
  end;
end;

// Binary Search in the Parser's Log for the starting entry inside the BlockStore's Zoom
function TImgBlockStore.binSearchStartEntry(): integer;
var
  chunkStart, chunkEnd, chunkSize, chunkMid : integer;
  searchOffset : Int64;
begin
  searchOffset := FZoom.SectOffset*FSectSize;
  chunkStart := 0;
  chunkEnd := Length(FParser.map)-1;
  chunkSize := chunkEnd-chunkStart+1;
  while chunkSize > 1 do begin
    chunkMid := chunkStart + (chunkSize div 2);
    if searchOffset < (FParser.map[chunkMid-1].offset+FParser.map[chunkMid-1].length) then chunkEnd := chunkMid-1
    else chunkStart := chunkMid;
    chunkSize := chunkEnd-chunkStart+1;
  end;
  result:=chunkStart;
end;

// Binary Search in the Parser's Log for the last entry making up the BlockStore's Zoom
function TImgBlockStore.binSearchEndEntry(): integer;
var
  chunkStart, chunkEnd, chunkSize, chunkMid : integer;
  searchOffset : Int64;
begin
  searchOffset := (FZoom.SectOffset+FZoom.SectCount)*FSectSize;
  chunkStart := 0;
  chunkEnd := Length(FParser.map)-1;
  chunkSize := chunkEnd-chunkStart+1;
  while chunkSize > 1 do begin
    chunkMid := chunkStart + (chunkSize div 2);
    if searchOffset <= FParser.map[chunkMid].offset then chunkEnd := chunkMid-1
    else chunkStart := chunkMid;
    chunkSize := chunkEnd-chunkStart+1;
  end;
  result:=chunkEnd;
end;

(* Calculate image blocks from map for fast drawing.
   Blocks will be stored in the imgBlocks[] array.
   Blocks contain type information and a color derived from the mix of types.
   Each block spans a multiple of the device sector size.
   This can lead to some unused blocks at the end of the grid, especially with
   small rescue devices, high number of imgBlocks or big sector size *)
procedure TImgBlockStore.buildBlocks;
var
  mapLen, mapBlock, imgBlock : integer;
  imgStartBlock, imgEndBlock : Int64;
  blockStatusMask : Longword;
  {$IFDEF VerboseProfiler}
  lTimeStart : TDateTime;
  {$ENDIF}
begin
  if not Assigned(FParser) then mapLen := 0
  else mapLen := Length(FParser.map);

  // adjust block array to requested size (FNumBlocks)
  if Length(FImgBlocks) <> FNumBlocks then SetLength(FImgBlocks, FNumBlocks);

  if FNumBlocks = 0 then exit; // cannot proceed this way

  if mapLen = 0 then begin // empty map (can have one of several reasons)
    FZoom.SectOffset:=0;
    FZoom.SectCount:=0; // reset zoom
    if (not Assigned(FParser)) or (not FParser.hasFile) then begin // no file or parser
      FillDword(FImgBlocks[0], Length(FImgBlocks), COLOR_NON_TRIED);
      exit;
    end;
  end;

  {$IFDEF VerboseProfiler}
  lTimeStart := Now();
  {$ENDIF}

  FillDword(FImgBlocks[0], Length(FImgBlocks), $00000000); //zero-out all blocks

  with FParser do begin
    // if zoom not yet set, or zoom is disabled, do sanitization
    if (FZoom.SectCount=0) or FNoZoom then sanitizeZoom();
    // iterate over map and collect block info
    for mapBlock := binSearchStartEntry() to binSearchEndEntry() do begin
      // process img blocks that intersect current map entry
      imgStartBlock := (map[mapBlock].offset-FZoom.SectOffset*FSectSize) div FByteInBlock;
      imgEndBlock := (map[mapBlock].offset+map[mapBlock].length-FZoom.SectOffset*FSectSize-1) div FByteInBlock;
      blockStatusMask := FBlockstatusMasks[ord(map[mapBlock].status)];
      for imgBlock := Max(0,imgStartBlock) to Min(FNumBlocks-1, imgEndBlock) do begin
        FImgBlocks[imgBlock]:=FImgBlocks[imgBlock] or blockStatusMask;
      end;
    end;

    // mark active block, i.e. block of current input read position
    if (rescueStatus.pos >= FZoom.SectOffset*FSectSize) and
       (rescueStatus.pos < (FZoom.SectOffset+FZoom.SectCount)*FSectSize) then begin
      imgBlock := (rescueStatus.pos-FZoom.SectOffset*FSectSize) div FByteInBlock;
      FImgBlocks[imgBlock]:=FImgBlocks[imgBlock] or MASK_ACTIVE;
    end;

    // go through blocks and build colors
    for imgBlock := 0 to FNumBlocks-1 do begin
      FImgBlocks[imgBlock]:=ColorizeBlockMask(FImgBlocks[imgBlock]);
    end;
  end;
  {$IFDEF VerboseProfiler}
  MainForm.DbgLog.Lines.Add(Format('[TImgBlockStore.buildBlocks] Duration: %d ms', [DateTimeToMilliseconds(Now() - lTimeStart)]));
  {$ENDIF}
end;

(* computes a color for a block, depending on whick block types exist in
   this block. The percentage of sectors of a specific block type has no influence.
   The weight of a color (block type) can be set by the user, but is at default:
   (least) non-tried, finished, non-trimmed, non-scraped, bad (highest)
   Each of these types influences the block color depending on its weight.
   This way, small local quantities of important (e.g. bad) blocks are easy
   to notice over less important (rescued, non-tried) ones.
   TODO: Optimize this for speed! A 6bit lookup table would do. *)
function TImgBlockStore.ColorizeBlockMask(mask: Longint) : Longint;
var i, b, g, r, totalweight, statusPresent : Longint;
begin
  b:=0; g:=0; r:=0; totalweight:=0;
  if not LongBool(mask and MASK_ALL_STATUSES) then begin
    result := mask or COLOR_UNDEFINED;
    exit;
  end;
  for i:= 0 to Length(MASKS)-1 do begin
    statusPresent:=Longint((mask and MASKS[i]) <> 0);
    b := b + statusPresent*WEIGHTS[i]^*((COLORS[i]^ and MASK_B) shr 16);
    g := g + statusPresent*WEIGHTS[i]^*((COLORS[i]^ and MASK_G) shr 8);
    r := r + statusPresent*WEIGHTS[i]^* (COLORS[i]^ and MASK_R);
    inc(totalweight, statusPresent*WEIGHTS[i]^);
  end;
  b := b div totalweight;
  g := g div totalweight;
  r := r div totalweight;
  result := mask or (b shl 16) or (g shl 8) or r;
end;

(* sets the device sector size and tries to maintain zoom as much as possible *)
procedure TImgBlockStore.setDevBSize(size: Int64);
var newOffset : Int64;
begin
  // do not allow setting the sector size without a Parser
  if (not Assigned(FParser)) then begin
    MainForm.DbgLog.Lines.Add('Cannot set device sector size: no data source!');
    MainForm.DSClick(self); // revert menu back to previous value
    exit;
  end;
  // catch invalid device sizes, but always allow default
  // sector size, in case of abnormal device size
  if (size <> DEF_BSIZE) and
    ((size <= 0) or (FParser.rescueStatus.devicesize mod size <> 0)) then begin
    MainForm.DbgLog.Lines.Add('Cannot set device sector size: ' +
      inttostr(size) + ' does not evenly divide device size.');
    MainForm.DSClick(self); // revert menu back to previous value
    exit;
  end;
  if (size = FSectSize) then exit; // nothing to do, exit

  newOffset := (FZoom.SectOffset*FSectSize) div size;
  FZoom.SectCount := ((FZoom.SectOffset+FZoom.SectCount)*FSectSize) div size - newOffset;
  FZoom.SectOffset := newOffset;
  FSectSize := size;
  sanitizeZoom();
  buildBlocks;
  FPONotifyObservers(self, ooChange, nil);
end;

procedure TImgBlockStore.SetLockOnCurPos(lock: Boolean);
begin
  FLockOnCurPos:=lock;
  if FLockOnCurPos then begin
    sanitizeZoom();
    buildBlocks;
    FPONotifyObservers(self, ooChange, nil);
  end;
end;

procedure TImgBlockStore.setNumBlocks(num: Longint);
begin
  FNumBlocks:=num;
  sanitizeZoom();
  buildBlocks;
  FPONotifyObservers(self, ooChange, nil);
end;

// shift the zoomed section by changing the offset
procedure TImgBlockStore.setSectOffset(offset: Int64);
begin
  FZoom.SectOffset:=offset;
  sanitizeZoom();
end;

// resize zoomed section
procedure TImgBlockStore.setSectCount(count: Int64);
begin
  FZoom.SectCount:=count;
  sanitizeZoom();
end;

//set zoomed section
procedure TImgBlockStore.setZoom(Offset, Count: Int64);
begin
  FZoom.SectOffset:=Offset;
  FZoom.SectCount:=Count;
  sanitizeZoom();
end;

// returns a bit mask for each known block type
function TImgBlockStore.BlockStatusToMask(status: char) : Longword;
begin
  case status of
    '?': result := MASK_NON_TRIED;
    '+': result := MASK_FINISHED;
    '*': result := MASK_NON_TRIMMED;
    '/': result := MASK_NON_SCRAPED;
    '-': result := MASK_BAD_SECT;
    'd': result := MASK_OUTSIDE_DOMAIN;
  else
    result := 0;
  end;
end;

// zoom in or out by some factor, centered around a given imgBlock
// factor <1 zooms in, factor >1 zooms out.
procedure TImgBlockStore.zoomToCenter(block: Longint; factor: double);
var
  zoomCenter, zoomLength, deviceSectors : Int64; // all values in sectors
begin
  if not Assigned(FParser) then exit;
  if not FParser.hasFile() then exit;
  zoomCenter:=getOffsetFromBlock(block) div FSectSize;
  deviceSectors:=FParser.rescueStatus.devicesize div FSectSize;
  if factor < 1.0 then begin // zoom in
    // if already at 1 sector per block -> dont zoom further
    if FSectInBlock=1 then exit;
    zoomLength:=Max(FnumBlocks, // not further than 1 sector/block
      Min(FZoom.SectCount-FnumBlocks, // reduce window by at least 1 sector/block
      FZoom.SectCount * Round(128*factor) div 128));
  end else begin // zoom out
    zoomLength:=Min(deviceSectors, FZoom.SectCount * Round(128*factor) div 128);
  end;
  FZoom.SectCount:=zoomLength;
  FZoom.SectOffset:=Max(0, Min(zoomCenter-(zoomLength div 2), deviceSectors-zoomLength));
  sanitizeZoom();
  buildBlocks;
  FPONotifyObservers(self, ooChange, nil);
end;

// Zooms the view in or out. After the zoom, the device position of srcBlock
// will be at destBlock.
procedure TImgBlockStore.zoomBlockToBlock(srcBlock, destBlock: Longint; factor: double);
var
  zoomLength, deviceSectors, srcBlockSect, newSectInBlock : Int64; // all values in sectors
begin
  if not Assigned(FParser) then exit;
  if not FParser.hasFile() then exit;

  deviceSectors:=FParser.rescueStatus.devicesize div FSectSize;
  zoomLength := Round(FZoom.SectCount*factor); // desired new window size
  if factor < 1.0 then begin // zooming in
    // if already at 1 sector/block -> do nothing
    if FSectInBlock = 1 then exit;
    // reduce window by at least 1 sector/block (needed for rounding reasons)
    zoomLength := Min(FZoom.SectCount-FnumBlocks, zoomLength);
  end;
  zoomLength := Max(FnumBlocks, zoomLength); // not further in than 1 sector/block
  zoomLength := Min(deviceSectors, zoomLength); // not further out than whole device
  srcBlockSect := getOffsetFromBlock(srcBlock) div FSectSize + FSectInBlock div 2;
  newSectInBlock := zoomLength div FNumBlocks + Int64((zoomLength mod FNumBlocks)<>0);
  FZoom.SectOffset := srcBlockSect - (newSectInBlock*destBlock + newSectInBlock div 2);
  FZoom.SectCount := zoomLength;
  sanitizeZoom();
  buildBlocks;
  FPONotifyObservers(self, ooChange, nil);
end;

// zoom into an area bounded by two blocks. Assumes block1 <= block2.
procedure TImgBlockStore.zoomInRange(block1, block2: Longint);
var
  zoomOffset, zoomLength, deviceSectors : Int64; // all values in sectors
begin
  if not Assigned(FParser) then exit;
  if not FParser.hasFile() then exit;
  if FSectInBlock=1 then exit; // if at 1 sector per block, dont zoom further
  if block1 > block2 then exit; // need block1 <= block2
  zoomOffset:=getOffsetFromBlock(block1) div FSectSize;
  zoomLength:=getOffsetFromBlock(block2) div FSectSize + FSectInBlock - zoomOffset;
  deviceSectors:=FParser.rescueStatus.devicesize div FSectSize;
  FZoom.SectCount:=zoomLength;
  FZoom.SectOffset:=Max(0, Min(zoomOffset, deviceSectors-zoomLength));
  sanitizeZoom();
  buildBlocks;
  FPONotifyObservers(self, ooChange, nil);
end;

end.

