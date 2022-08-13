(*
   BlockInspector.pas - Grid Block Inspector unit

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

unit BlockInspector;

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Grids, Spin, ExtCtrls, Shared, Parser, ImgBlockStore;

type

  { TBlockForm }

  TBlockForm = class(TForm, IFPObserver)
    BICol1Panel: TPanel;
    BICol2Panel: TPanel;
    BICol3Panel: TPanel;
    BlockEdit: TSpinEdit;
    BlockInfo: TGroupBox;
    EditBadSectors: TEdit;
    EditNonScrapedSectors: TEdit;
    EditNonTriedSectors: TEdit;
    EditRescuedSectors: TEdit;
    EditBlockInfo: TEdit;
    EditNonTrimmedSectors: TEdit;
    lblBad: TLabel;
    lblNonScraped: TLabel;
    lblNonTried: TLabel;
    lblNonTrimmed: TLabel;
    lblRescued: TLabel;
    BottomPanel: TPanel;
    TopPanel: TPanel;
    CloseBIButton: TButton;
    lblBlockHeadline: TLabel;
    MapEntryList: TStringGrid;
    TrackCurrentCheckBox: TCheckBox;
    procedure BlockEditChange(Sender: TObject);
    procedure FormHide(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure lblBlockHeadlineDblClick(Sender: TObject);
    procedure MapEntryListHeaderSized(Sender: TObject; IsColumn: Boolean; Index: Integer);
    procedure CloseBIButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure TrackCurrentCheckBoxChange(Sender: TObject);
  private
    FBlock : integer;
    FParser : TMapParser;
    FGridBlockStore : TImgBlockStore;
    procedure SetBlock(block : integer);
  public
    property Block : integer read FBlock write SetBlock;
    procedure DoUpdate(clearSel : boolean);
    Procedure FPOObservedChanged(ASender : TObject;
      Operation : TFPObservedOperation; Data : Pointer);
  end; 

var
  BlockForm: TBlockForm;

implementation
uses GUI, Math;

{$R *.lfm}

procedure TBlockForm.FormCreate(Sender: TObject);
begin
  FParser:=MainForm.Parser;
  FGridBlockStore:=MainForm.GridBlockStore;
  FGridBlockStore.AttachObserver(self);
end;

procedure TBlockForm.FormDestroy(Sender: TObject);
begin
  FGridBlockStore.DetachObserver(self);
  FGridBlockStore:=nil;
end;

procedure TBlockForm.TrackCurrentCheckBoxChange(Sender: TObject);
begin
  if TrackCurrentCheckBox.Checked then DoUpdate(false);
end;

Procedure TBlockForm.FPOObservedChanged(ASender : TObject;
  Operation : TFPObservedOperation; Data : Pointer);
begin
  if (ASender = FGridBlockStore) then begin
    DoUpdate(false);
  end;
end;

// public method to set the block. Called from the Main GUI.
// this is not called when the BI is notified.
procedure TBlockForm.SetBlock(block : integer);
var
  numBlocks : Longint;
  activeblock : Int64;
begin
  activeblock := FGridBlockStore.activeBlock();
  numBlocks := FGridBlockStore.NumBlocks;
  FBlock := max(0, min(block, numBlocks));  // sanitize to bounds
  // Disable Cur. Pos. tracking if requested, sanitized block is not active block
  if (FBlock <> activeblock) then TrackCurrentCheckBox.Checked:=false;
  // update the Form, clear the user's list selection
  DoUpdate(true);
end;

(* Updates the Block Inspector Form. If the TrackCurrentCheckBox is checked,
   it will change FBlock accordingly and display the current block instead.
   If clearSel is true, the selected row in the mapEntry list is reset to
   the first item. *)
procedure TBlockForm.DoUpdate(clearSel : boolean);
var
  mapEntry, mapLen, row : Longint;
  actblock, bStart, bEnd, leStart, leEnd, overlap : Int64;
  rs : TRescueStatus; // only for counting sectors here
  BlockEditChangeEvent : TNotifyEvent;
begin
  with FGridBlockStore do begin
    // check if file is open
    if not Assigned(Parser) or not Parser.hasFile() then begin
      MapEntryList.RowCount:=1; // clear MapEntryList
      Hide;  // close BI window
      exit;
    end;

    FBlock := EnsureRange(block, 0, NumBlocks-1);  // sanitize to bounds
    // adjust FBlock if current pos tracking is enabled and activeblock is in bounds
    actblock:=activeBlock();
    if TrackCurrentCheckBox.Checked and (FBlock <> actblock) and
      (activeblock >=0) and (activeBlock < NumBlocks) then begin
      FBlock := actblock;
      BlockEdit.Value:=FBlock; // calls doUpdate again and updates the MainForm
      exit;
    end;

    // set the spinEdit bounds, deactivate its event while doing so
    BlockEditChangeEvent:=BlockEdit.OnChange;
    BlockEdit.OnChange:=nil;
    BlockEdit.Value:=FBlock;
    BlockEdit.MaxValue:=Min(NumBlocks-1, getBlockFromOffset(Parser.rescueStatus.devicesize-1));
    BlockEdit.OnChange:=BlockEditChangeEvent;

    rs:=emptyRescueStatus;
    bStart := getOffsetFromBlock(FBlock);
    bEnd := max(bStart, min(Parser.rescueStatus.deviceSize, getOffsetFromBlock(block+1)));

    EditBlockInfo.Text:='Start: ' + SFORMATS[lblBlockHeadline.Tag]^.SizeStr(bStart) +
      '  Size: ' + SFORMATS[lblBlockHeadline.Tag]^.SizeStr(bEnd-bStart) +
      ' (' + SF_SECT.SizeStr(bEnd-bStart) + 'ectors)';

    with Parser do begin
      mapLen := Length(map);
      // go through map and make list entries
      row := 0;
      for mapEntry := 0 to mapLen-1 do begin
        leStart:=map[mapEntry].offset;
        leEnd:=leStart+map[mapEntry].length;

        if leEnd <= bStart then continue;
        if leStart >= bEnd then break; // this and all following entries start after FBlock

        inc(row);
        if MapEntryList.RowCount <= row then MapEntryList.RowCount:=row+1; // need more space
        MapEntryList.Cells[0, row]:=BlockStatusToString(map[mapEntry].status);
        // this map block is current rescue pos
        if (rescueStatus.pos >= leStart) and (rescueStatus.pos < leEnd) then
          MapEntryList.Cells[0, row]:='> '+MapEntryList.Cells[0, row];
        MapEntryList.Cells[1, row]:=SF_HEX.SizeStr(map[mapEntry].offset)+
          ' ('+SF_FLOAT.SizeStr(map[mapEntry].offset)+')';
        MapEntryList.Cells[2, row]:=SF_HEX.SizeStr(map[mapEntry].length)+
          ' ('+SF_FLOAT.SizeStr(map[mapEntry].length)+')';
        overlap:=BlockOverlap(leStart, leEnd, bStart, bEnd);
        case map[mapEntry].status of
          '?' : inc(rs.nontried, overlap);
          '+' : inc(rs.rescued, overlap);
          '*' : inc(rs.nontrimmed, overlap);
          '/' : inc(rs.nonscraped, overlap);
          '-' : inc(rs.bad, overlap);
        end;
      end;
      MapEntryList.RowCount:=row+1;
      if clearSel then MapEntryList.Row:=1;
    end; // with Parser
    EditNontriedSectors.Text:=SF_SECT.SizeStr(rs.nontried);
    EditRescuedSectors.Text:=SF_SECT.SizeStr(rs.rescued);
    EditNonTrimmedSectors.Text:=SF_SECT.SizeStr(rs.nontrimmed);
    EditNonScrapedSectors.Text:=SF_SECT.SizeStr(rs.nonscraped);
    EditBadSectors.Text:=SF_SECT.SizeStr(rs.bad);
  end; // with FGridBlockStore
end;

// user closes the form
procedure TBlockForm.CloseBIButtonClick(Sender: TObject);
begin
  Hide;
end;

// The block number in the spin-edit has been changed
procedure TBlockForm.BlockEditChange(Sender: TObject);
begin
  MainForm.SelectBlock(BlockEdit.Value);
  SetBlock(BlockEdit.Value);
end;

// prevent when hiding the form, unset the mainform's selected block and
// prevent further trakcing of the active block
procedure TBlockForm.FormHide(Sender: TObject);
begin
  MainForm.SelectBlock(-1);
  TrackCurrentCheckBox.Checked:=false;
end;

procedure TBlockForm.FormShow(Sender: TObject);
begin
  self.Position:=poDesigned; // make the form appear at its prev pos when reopened
end;

// switch the boundaries line size format
procedure TBlockForm.lblBlockHeadlineDblClick(Sender: TObject);
begin
  lblBlockHeadline.Tag := (lblBlockHeadline.Tag + 1) mod 3;
  doUpdate(false);
end;

// prevent 'Status' column from becoming invisible
procedure TBlockForm.MapEntryListHeaderSized(Sender: TObject; IsColumn: Boolean;
  Index: Integer);
begin
  if MapEntryList.Columns.Items[0].Width < MapEntryList.Columns.Items[0].MinSize then
     MapEntryList.Columns.Items[0].Width := MapEntryList.Columns.Items[0].MinSize;
end;


end.

