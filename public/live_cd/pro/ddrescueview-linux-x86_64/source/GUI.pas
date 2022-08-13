(*
   GUI.pas - Main GUI unit

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

unit GUI;

interface

uses
  SysUtils, Classes, Graphics, graphtype,
  Controls, Forms, Dialogs, Menus, ExtCtrls, ComCtrls, StdCtrls, Buttons,
  Shared, Parser, ImgBlockStore;

type
  TMouseCapImage = class(TImage)
  published
    property MouseCapture; // allows the use of this property.
  end;
  { TMainForm }
  TMainForm = class(TForm, IFPObserver, IAttachableObserved)
    BottomPanel: TPanel;
    CommentsMemo: TMemo;
    EditBadSectors: TEdit;
    EditOutsideDomain: TEdit;
    EditPending: TEdit;
    EditRescued: TEdit;
    EditDomainSize: TEdit;
    EditNontried: TEdit;
    EditNontrimmed: TEdit;
    EditNonscraped: TEdit;
    lblBadSectors: TLabel;
    lblDomainSize: TLabel;
    lblOutsideDomain: TLabel;
    lblPending: TLabel;
    lblRescued: TLabel;
    miCenterCurPos: TMenuItem;
    SaveDialog1: TSaveDialog;
    saveFormImage: TMenuItem;
    miContigDom: TMenuItem;
    miOpenDomFile: TMenuItem;
    RSCol1Panel: TPanel;
    RSCol2Panel: TPanel;
    EditErrorCount: TEdit;
    EditInputSize: TEdit;
    EditCurrentPos: TEdit;
    RescueStatusBox: TGroupBox;
    BtnCloseZoomBar: TSpeedButton;
    lblErrorCount: TLabel;
    lblCurrentPos: TLabel;
    lblInputSize: TLabel;
    MainPanel: TPanel;
    miShowMemos: TMenuItem;
    BottomHSplitter: TSplitter;
    MainVSplitter: TSplitter;
    RSCol3Panel: TPanel;
    ShapeOutsideDomain: TShape;
    RetryTimer: TTimer;
    View1: TMenuItem;
    miSettings: TMenuItem;
    miZoomBar: TMenuItem;
    ZoomImage: TImage;
    miSectSize: TMenuItem;
    DS512: TMenuItem;
    DS2048: TMenuItem;
    DSCustom: TMenuItem;
    DS4096: TMenuItem;
    Extras1: TMenuItem;
    ShapeFinished: TShape;
    ShapeNonTried: TShape;
    ShapeBad: TShape;
    ShapeNonScraped: TShape;
    ShapeNonTrimmed: TShape;
    lblNonTried: TLabel;
    lblNonScraped: TLabel;
    lblNonTrimmed: TLabel;
    ShapeActive: TShape;
    ZoomPanel: TPanel;
    miRefresh: TMenuItem;
    miAbout: TMenuItem;
    TopPanel: TPanel;
    TopPiePanel: TPanel;
    PieImage: TImage;
    MainMenu1: TMainMenu;
    FileMenu: TMenuItem;
    miOpenMapfile: TMenuItem;
    miExit: TMenuItem;
    BlockImage: TImage;
    OpenDialog1: TOpenDialog;
    StatusBar: TStatusBar;
    DbgLog: TMemo;
    miCloseMapfile: TMenuItem;
    OptionsMenu: TMenuItem;
    miAutorefresh: TMenuItem;
    miAR5sec: TMenuItem;
    miAR10sec: TMenuItem;
    miAR30sec: TMenuItem;
    miAR1min: TMenuItem;
    miARoff: TMenuItem;
    miAR2mins: TMenuItem;
    miAR5mins: TMenuItem;
    updateTimer: TTimer;
    GridPanel: TPanel;
    miPrefixes: TMenuItem;
    miBinaryPrefixes: TMenuItem;
    miDecimalPrefixes: TMenuItem;
    miGridsize: TMenuItem;
    mi4px: TMenuItem;
    mi6px: TMenuItem;
    mi8px: TMenuItem;
    mi10px: TMenuItem;
    mi12px: TMenuItem;
    mi14px: TMenuItem;
    mi16px: TMenuItem;
    mi20px: TMenuItem;
    mi24px: TMenuItem;
    ApplicationEvents1: TApplicationProperties;
    procedure BlockImageMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure BlockImageMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure BlockImageMouseWheel(Sender: TObject; Shift: TShiftState;
      WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
    procedure BtnCloseZoomBarClick(Sender: TObject);
    procedure DbgLogDblClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure miCenterCurPosClick(Sender: TObject);
    procedure miContigDomClick(Sender: TObject);
    procedure miOpenDomFileClick(Sender: TObject);
    procedure rsLabelClick(Sender: TObject);
    procedure miSettingsClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure BlockImageMouseDown(Sender: TObject; Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
    procedure DSClick(Sender: TObject);
    procedure FormDropFiles(Sender: TObject; const FileNames: array of String);
    procedure GridPanelResize(Sender: TObject);
    procedure miCloseMapfileClick(Sender: TObject);
    procedure miShowMemosClick(Sender: TObject);
    procedure miZoomBarClick(Sender: TObject);
    procedure saveFormImageClick(Sender: TObject);
    procedure setGridSize(Sender: TObject);
    procedure miOpenMapfileClick(Sender: TObject);
    procedure miExitClick(Sender: TObject);
    procedure miRefreshClick(Sender: TObject);
    procedure autoParseClick(Sender: TObject);
    procedure UnitFormatClick(Sender: TObject);
    procedure miAboutClick(Sender: TObject);
    procedure miClearlogClick(Sender: TObject);
    procedure ApplicationEvents1ShowHint(var HintStr: String;
      var CanShow: Boolean; var HintInfo: THintInfo);
    procedure ZoomImageMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure ZoomImageMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure ZoomImageMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    { Private declarations }
    FParser : TMapParser;
    FGridBlockStore : TImgBlockStore;
    FZoomBlockStore : TImgBlockStore;
    Fhblocks : integer;
    Fvblocks : integer;
    FgridSize : integer;
    FZoomBarScroll : boolean;
    FZoomBarScrollOrigin : Integer;
    FZoomBarSelectStart, FZoomBarSelectEnd : Longint;
    FZoomBehaviour : boolean; // 0: zoom to center, 1: to mouse pos
    FSelectedBlock : Longint;
    FBlockSelectStart, FBlockSelectEnd : Longint;
    drawBlocks : procedure of object;
    procedure drawBlocksSlow;
    procedure drawBlocksFast;
    procedure drawTopBarItems;
    procedure drawZoomImage;
    procedure updateStatusTexts;
    procedure ParseCommandline;
    procedure ParseCommandline2;
    procedure OpenCmdlineFiles(Data: PtrInt);
    procedure OpenFile(path: String);
    procedure OpenDomainFile(path: String);
    function buildStatusBarText(path : String; refreshing: Boolean; opening: Boolean) : String;
    function getImgBlockRect(imgBlock : integer):TRect;
    function resizeRect(rect : TRect; modSize : integer):TRect;
    function BlockImageMouseActionAllowedGetBlock(X, Y: Integer) : Longint;
  public
    { Public declarations }
    Procedure FPOObservedChanged(ASender : TObject;  // Observer role
      Operation : TFPObservedOperation; Data : Pointer);
    procedure AttachObserver(AObserver : TObject);   // Observed role
    procedure DetachObserver(AObserver : TObject);   // Observed role
    procedure SelectBlock(block : integer);
    procedure setDrawFunction(report: Boolean; which : integer);
    procedure setZoomBehaviour(behaviour : boolean);
    function GridXYToBlock(X, Y : integer) : integer;
    property Parser : TMapParser read FParser;
    property GridBlockStore : TImgBlockStore read FGridBlockStore;
    property ZoomBlockStore : TImgBlockStore read FZoomBlockStore;
  end;


var
  MainForm: TMainForm;

implementation
uses Math, BlockInspector, About, Settings;

{$R *.lfm}

// Invocation syntax is:
// ddrescueview <options> [[-m domain-mapfile] rescue-mapfile]
// where <options> can be:
//   -r (off|5s|10s|30s|1m|2m|5m)  Set refresh interval, e.g. -r 30s
//   -lp                           Show (bottom) log panel on start
//   -gs (4|6|..|24)               Set grid size on start, e.g. -gs 10
//   -ub                           Use binary unit prefixes instead of decimal
//   -x left                       X position of window on screen, in pixels
//   -y top                        Y position of window on screen, in pixels
//   -w width                      ClientWidth of window, in pixels
//   -h height                     ClientHeight of window, in pixels
//   -safe                         Turn off optimizations

procedure TMainForm.ParseCommandline;
var
  i: integer;
begin
  for i:=1 to ParamCount do if (ParamStr(i) = '-safe') then setDrawFunction(false, 1);
end;

procedure TMainForm.ParseCommandline2;
var
  i, paramRefresh, paramGridsize, paramLeft, paramTop, paramW, paramH : integer;
begin
  // initialize the index of the parameter values to -1 (not found)
  paramRefresh:=-1;
  paramGridsize:=-1;
  paramLeft:=-1; paramTop:=-1;
  paramW:=-1; paramH:=-1;

  // find the index of various parameter values
  for i:=1 to ParamCount do begin
    if (ParamStr(i) = '-r') and (i < ParamCount) then paramRefresh:=i+1
    else if (ParamStr(i) = '-lp') then miShowMemos.Click
    else if (ParamStr(i) = '-gs') and (i < ParamCount) then paramGridsize:=i+1
    else if (ParamStr(i) = '-ub') then miBinaryPrefixes.Click
    else if (ParamStr(i) = '-x') and (i < ParamCount) then paramLeft:=i+1
    else if (ParamStr(i) = '-y') and (i < ParamCount) then paramTop:=i+1
    else if (ParamStr(i) = '-w') and (i < ParamCount) then paramW:=i+1
    else if (ParamStr(i) = '-h') and (i < ParamCount) then paramH:=i+1;
  end;

  // apply the parameters which have been found on the cmdline
  // window position
  if paramW <> -1 then Width:=StrToIntDef(ParamStr(paramW), Width);
  if paramH <> -1 then Height:=StrToIntDef(ParamStr(paramH), Height);
  if paramLeft <> -1 then Left:=StrToIntDef(ParamStr(paramLeft), Left);
  if paramTop <> -1 then Top:=StrToIntDef(ParamStr(paramTop), Top);


  if paramRefresh <> -1 then begin // refresh interval
     if ParamStr(paramRefresh) = 'off' then miARoff.Click
     else if ParamStr(paramRefresh) = '5s' then miAR5sec.Click
     else if ParamStr(paramRefresh) = '10s' then miAR10sec.Click
     else if ParamStr(paramRefresh) = '30s' then miAR30sec.Click
     else if ParamStr(paramRefresh) = '1m' then miAR1min.Click
     else if ParamStr(paramRefresh) = '2m' then miAR2mins.Click
     else if ParamStr(paramRefresh) = '5m' then miAR5mins.Click;
  end;
  if paramGridsize <> -1 then begin // gridsize
     if ParamStr(paramGridsize) = '4' then mi4px.Click
     else if ParamStr(paramGridsize) = '6' then mi6px.Click
     else if ParamStr(paramGridsize) = '8' then mi8px.Click
     else if ParamStr(paramGridsize) = '10' then mi10px.Click
     else if ParamStr(paramGridsize) = '12' then mi12px.Click
     else if ParamStr(paramGridsize) = '14' then mi14px.Click
     else if ParamStr(paramGridsize) = '16' then mi16px.Click
     else if ParamStr(paramGridsize) = '20' then mi20px.Click
     else if ParamStr(paramGridsize) = '24' then mi24px.Click;
  end;
end;

procedure TMainForm.OpenCmdlineFiles(Data: PtrInt);
var
  i, paramDom : integer;
  wantContig : boolean;
begin
  // initialize the index of the parameter values to -1 (not found)
  paramDom:=-1;
  wantContig:=false;
  // find the index of various parameter values
  for i:=1 to ParamCount do begin
    if (ParamStr(i) = '-m') and (i < ParamCount) then paramDom:=i+1
    else if (ParamStr(i) = '-contig') then wantContig:=true;
  end;

  // open the map file(s). The regular mapfile must be the last argument.
  if (ParamCount > 0) and FileExists(ParamStr(ParamCount)) and (ParamCount <> paramDom) then begin
    OpenFile(ParamStr(ParamCount));
    if (paramDom <> -1) then begin // if also supplied, open domain mapfile, too.
      if FileExists(ParamStr(paramDom)) then begin
        OpenDomainFile(ParamStr(paramDom));
        if wantContig then miContigDom.Click;
      end;
    end;
  end;
end;

procedure TMainForm.AttachObserver(AObserver : TObject);
begin
  FPOAttachObserver(AObserver);
end;

procedure TMainForm.DetachObserver(AObserver : TObject);
begin
  FPODetachObserver(AObserver);
end;

// initialization on program start
procedure TMainForm.FormCreate(Sender: TObject);
begin
  DoubleBuffered:=true;
  TopPanel.DoubleBuffered:=true;
  MainPanel.DoubleBuffered:=true;
  StatusBar.DoubleBuffered:=true;
  BottomPanel.DoubleBuffered:=true;
  CommentsMemo.DoubleBuffered:=true;
  DbgLog.DoubleBuffered:=true;

  FgridSize := 8;
  FZoomBehaviour:=true;

  ZoomImage.Align:=alNone;  // alClient only useful for Form Designer
  BlockImage.Align:=alNone; // at x-platform designtime, but disruptive at runtime
  PieImage.Picture.Bitmap.SetSize(PieImage.Width, PieImage.Height);

  FParser := TMapParser.Create;

  FZoomBlockStore := TImgBlockStore.Create(self);
  FZoomBlockStore.NoZoom:=true; // ensure that the zoom bar never zooms in
  FZoomBlockStore.Parser := FParser; // must be set before FGridBlockStore's
  FZoomBlockStore.AttachObserver(self);

  FGridBlockStore := TImgBlockStore.Create(self);
  FGridBlockStore.Parser := FParser;
  FGridBlockStore.AttachObserver(self);

  FSelectedBlock:=-1;
  ParseCommandline; //first parse -safe cmdline arg (must be handled pre-Show)
  GridPanelResize(self); // to trigger initial block display

  Constraints.MinHeight:=Height+(MainVSplitter.MinSize-GridPanel.Height);
  // Put the parsing of cmdline params (and acting upon) to the end of the msg queue.
  // Those actions will be executed after the form is fully shown and has processed
  // all events.
  Application.QueueAsyncCall(@OpenCmdlineFiles, 0);
end;

// destruction when form is closed
procedure TMainForm.FormDestroy(Sender: TObject);
begin
  FGridBlockStore.DetachObserver(self);
  FZoomBlockStore.DetachObserver(self);
  FreeAndNil(FGridBlockStore);
  FreeAndNil(FZoomBlockStore);
  FreeAndNil(FParser);
end;

// Mainform is notified about a change
procedure TMainForm.FPOObservedChanged(ASender: TObject;
  Operation: TFPObservedOperation; Data: Pointer);
begin
  if (ASender = FGridBlockStore) then begin
    if miCloseMapfile.Enabled<>FParser.hasFile then begin
      miCloseMapfile.Enabled:=FParser.hasFile;
      miOpenDomFile.Enabled:=FParser.hasFile;
      miRefresh.Enabled:=FParser.hasFile;
    end;
    miContigDom.Enabled:=FParser.hasDomFile;
    if lblDomainSize.Visible<>FParser.hasDomFile and not FParser.ContiguousDomain then begin
      lblDomainSize.Visible:=FParser.hasDomFile and not FParser.ContiguousDomain;
      EditDomainSize.Visible:=lblDomainSize.Visible;
      ShapeOutsideDomain.Visible:=lblDomainSize.Visible;
      lblOutsideDomain.Visible:=lblDomainSize.Visible;
      EditOutsideDomain.Visible:=lblDomainSize.Visible;
    end;
    DSClick(self);
    drawBlocks;
    drawTopBarItems;
    updateStatusTexts;
  end;
  if (ASender = FZoomBlockStore) then drawZoomImage;
end;

// file(s) have been dropped onto the form. Open the first file.
procedure TMainForm.FormDropFiles(Sender: TObject; const FileNames: array of String);
begin
  OpenFile(FileNames[0]);
end;

// Prevent BottomPanel oversizing into grid panel and the TSplitter from getting
// between bottom panel and status bar, using an adventurous mix of workarounds.
procedure TMainForm.FormResize(Sender: TObject);
begin
  if miShowMemos.Checked then begin
    MainVSplitter.Top:=0;
    MainVSplitter.ResizeControl:=BottomPanel;
    if GridPanel.Height < MainVSplitter.MinSize then begin
      MainVSplitter.MoveSplitter(MainVSplitter.MinSize - GridPanel.Height);
      BottomPanel.Height:=MainPanel.ClientHeight-MainVSplitter.MinSize-MainVSplitter.Height;
    end;
    if BottomPanel.Height < MainVSplitter.MinSize then BottomPanel.Height := MainVSplitter.MinSize;
  end;
end;

procedure TMainForm.FormShow(Sender: TObject);
begin
  ParseCommandline2;
  self.OnShow:=nil;
end;

// handles things that need manual handling when the GridPanel resizes.
procedure TMainForm.GridPanelResize(Sender: TObject);
var
  hBlocksBefore, vBlocksBefore: integer;
  nBlocksChanged, zoomImageChanged: Boolean;
begin
  FormResize(self); // for VSplitter issue
  (* BlockImage resizing *)
  hBlocksBefore := FhBlocks;
  vBlocksBefore := FvBlocks;
  BlockImage.Width:=GridPanel.ClientWidth - ((GridPanel.ClientWidth-1) mod FgridSize);
  BlockImage.Height:=GridPanel.ClientHeight - ((GridPanel.ClientHeight-1) mod FgridSize);
  FhBlocks := BlockImage.Width div FgridSize;  // number of horizontal blocks in display
  FvBlocks := BlockImage.Height div FgridSize; // same for vertical

  nBlocksChanged := (hBlocksBefore <> Fhblocks) or (vBlocksBefore <> FvBlocks);
  if nBlocksChanged then begin
    FBlockSelectStart:=-1; FBlockSelectEnd:=FhBlocks*FvBlocks;
    BlockImage.Picture.Bitmap.SetSize(BlockImage.Width, BlockImage.Height);
    if drawBlocks=nil then setDrawFunction(false, 0); // decide here which draw function to use
    FGridBlockStore.NumBlocks:=FhBlocks*FvBlocks; // will trigger update
  end;

  (* ZoomImage resizing: Ensure same height as BlockImage *)
  zoomImageChanged := (ZoomImage.Height<>BlockImage.Height) or
    (FZoomBlockStore.NumBlocks<>ZoomImage.Height);
  if zoomImageChanged then begin
    ZoomImage.Height:=BlockImage.Height;
    ZoomImage.Width:=ZoomPanel.ClientWidth;
    ZoomImage.Picture.Bitmap.SetSize(ZoomImage.Width, ZoomImage.Height);
    FZoomBarSelectStart := -1; FZoomBarSelectEnd := ZoomImage.Height;
    FZoomBlockStore.NumBlocks:=ZoomImage.Height;  // will trigger update
  end;
  if zoomImageChanged or nBlocksChanged then SelectBlock(-1); // unselect block
  Constraints.MinWidth:=RescueStatusBox.Left+RescueStatusBox.Width+64+4;
end;


// update a lot of textual info on the form.
procedure TMainForm.updateStatusTexts;
var
  rs : TRescueStatus;
  newCap : String; // new caption for main window
begin
  CommentsMemo.Clear;
  newCap := PROGRAM_TITLE;
  with FParser do begin
    if hasFile then begin
      newCap := newCap + ' [' + FilePart(MapFileName);
      if hasDomFile then newCap := newCap + ' / ' + FilePart(DomFileName);
      newCap := newCap + ']';
      rs:=rescueStatus;
      CommentsMemo.Lines.AddStrings(CommentLines);
      RescueStatusBox.Caption:='Rescue status:  '+OperationToText(rs.curOperation, rs.current_pass);
      StatusBar.Panels.Items[0].Text:=buildStatusBarText(MapFileName, false, false);
    end else begin
      rs:=emptyRescueStatus;
      RescueStatusBox.Caption:='Rescue status';
      StatusBar.Panels.Items[0].Text:='No file opened';
    end;
  end;
  if Caption<>newCap then begin
    Caption:=newCap;
    Application.Title:=newCap;
  end;
  EditInputSize.Text:=SFORMATS[lblInputSize.Tag]^.SizeStr(rs.devicesize);
  EditDomainSize.Text:=SFORMATS[lblDomainSize.Tag]^.SizeStr(rs.devicesize-rs.outsidedomain);
  EditErrorCount.Text:=IntToStr(rs.errors);
  EditPending.Text:=SFORMATS[lblPending.Tag]^.SizeStr(rs.nontried + rs.nonscraped + rs.nontrimmed);
  EditRescued.Text:=SFORMATS[lblRescued.Tag]^.SizeStr(rs.rescued);
  EditOutsideDomain.Text:=SFORMATS[lblOutsideDomain.Tag]^.SizeStr(rs.outsidedomain);
  EditBadSectors.Text:=SFORMATS[lblBadSectors.Tag]^.SizeStr(rs.bad);
  EditCurrentPos.Text:=SFORMATS[lblCurrentPos.Tag]^.SizeStr(rs.pos);
  EditNontried.Text:=SFORMATS[lblNonTried.Tag]^.SizeStr(rs.nontried);
  EditNontrimmed.Text:=SFORMATS[lblNonTrimmed.Tag]^.SizeStr(rs.nontrimmed);
  EditNonscraped.Text:=SFORMATS[lblNonScraped.Tag]^.SizeStr(rs.nonscraped);
end;

procedure TMainForm.OpenFile(path: String);
begin
  StatusBar.Panels[0].Text:=buildStatusBarText(path, false, true);
  Application.ProcessMessages;
  FParser.OpenFile(path);
end;

procedure TMainForm.OpenDomainFile(path: String);
begin
  StatusBar.Panels[0].Text:=buildStatusBarText(FParser.MapFileName, false, true);
  Application.ProcessMessages;
  FParser.OpenDomainFile(path);
end;

function TMainForm.buildStatusBarText(path : String; refreshing: Boolean; opening: Boolean) : String;
begin
  result:=path;
  if opening then result := result + ' [Opening...]';
  if refreshing then result := result + ' [Updating...]';
end;

// maps a X,Y pixel location on the block image to a block number
function TMainForm.GridXYToBlock(X, Y : integer) : integer;
begin
  result := (Y div FgridSize)*Fhblocks + X div FgridSize;
end;

// constructs a TRect containing the requested Image Block
function TMainForm.getImgBlockRect(imgBlock : integer):TRect;
var hblock, vblock: integer;
begin
   vblock := imgBlock div Fhblocks;
   hblock := imgBlock mod Fhblocks;
   result.Left:=1+hblock*FgridSize;
   result.Top:=1+vblock*FgridSize;
   result.Right:=result.Left+FgridSize-1;
   result.Bottom:=result.Top+FgridSize-1;
end;

// shrinks/extends the TRect by modSize pixels on each side
function TMainForm.resizeRect(rect : TRect; modSize : integer):TRect;
begin
   result:=rect;
   result.Left-=modSize; result.Top-=modSize;
   result.Right+=modSize; result.Bottom+=modSize;
end;

// select the drawing function according to rawImage properties
// which=0: determine automatically, which=1: use slow, which=2: use fast
procedure TMainForm.setDrawFunction(report: Boolean; which : integer);
var
  Desc: TRawImageDescription;
  capable: Boolean = false;
begin
  case which of
    0: begin
        capable:=true;
        Desc := BlockImage.Picture.Bitmap.RawImage.Description;
        if report then DbgLog.Lines.Add(Desc.AsString);
        if Desc.PaletteColorCount > 0 then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! Images uses palette colors.');
        end;
        if Desc.Format <> ricfRGBA then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! Color format is not RGB(A)-based.');
        end;
        if Desc.BitsPerPixel < 24 then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! Pixel size < 24 bits.');
        end;
        if (Desc.BitsPerPixel mod 8) <> 0  then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! Pixel size not divisible by 8.');
        end;
        if Desc.Depth < 24 then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! Color depth < 24 bits.');
        end;
        if (Desc.BluePrec <> 8) or (Desc.GreenPrec <> 8) or (Desc.RedPrec <> 8) then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! One or more color components != 8 bit.');
        end;
        if Desc.LineOrder <> riloTopToBottom then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! Line order is not top-to-bottom.');
        end;
        if Desc.BytesPerLine < LongWord(Desc.BitsPerPixel div 8)*Desc.Width then begin
          capable:=false;
          if report then DbgLog.Lines.Add('!! BytesPerLine sanity check failed.');
        end;
       end;
    1: capable:=false;
    2: capable:=true;
  end;

  if capable then begin
    DbgLog.Lines.Add('Using fast drawing routines.');
    drawBlocks := @drawBlocksFast;
  end else begin
    DbgLog.Lines.Add('Using slow drawing routines.');
    drawBlocks := @drawBlocksSlow;
  end;
end;

procedure TMainForm.setZoomBehaviour(behaviour: boolean);
begin
  FZoomBehaviour:=behaviour;
end;

//get block colors from GridBlockStore and draw them on the BlockImage
procedure TMainForm.drawBlocksSlow;
var iImgBlock : Longint;
    curImgBlock : Longword;
begin
  with BlockImage.Canvas do begin
    Brush.Color := COLOR_UNDEFINED; // color for the gridlines
    FillRect(ClipRect);
    for iImgBlock := 0 to FGridBlockStore.NumBlocks-1 do begin
      curImgBlock := FGridBlockStore[iImgBlock]; // get current block
      Brush.Color:=blendColors(COLOR_HIGHLIGHT, curImgBlock and $ffffff,
        112*LongInt(not InRangeEx(iImgBlock, FBlockSelectStart, FBlockSelectEnd)));
      if LongBool(curImgBlock and MASK_ACTIVE) then begin // current pos marker
        Pen.Color := COLOR_ACTIVE;
        Rectangle(resizeRect(getImgBlockRect(iImgBlock), 1));
      end else FillRect(getImgBlockRect(iImgBlock));
    end;
    if FSelectedBlock >= 0 then begin
      Brush.Color := COLOR_SELECTED;
      FrameRect(resizeRect(getImgBlockRect(FSelectedBlock), 2));
    end;
  end;
  BlockImage.Refresh;
  //DbgLog.Lines.Add('image blocks drawn: '+inttostr(FGridBlockStore.NumBlocks));
end;

procedure TMainForm.drawBlocksFast;
var
  x, y, xs, ys, ye, iImgBlock, curBlockData, colorRowSize,
  actBlock, numBlocks : Longint;
  RedMemOffs, GreenMemOffs, BlueMemOffs : Longint;
  BytePtr, PixPtr, DataPtr : PByte;
  RawImage: TRawImage;
  vblock, hblock, BytePerLine, BytePerPix : Longint;
  colorRow : array of Byte;
begin
  actblock:=-1;
  try
    BlockImage.Picture.Bitmap.BeginUpdate(false);
    RawImage := BlockImage.Picture.Bitmap.RawImage;
    DataPtr := RawImage.Data;
    BytePerLine := RawImage.Description.BytesPerLine;
    BytePerPix := RawImage.Description.BitsPerPixel div 8;
      //todo: use Desc.GetRGBIndices() ?
    if RawImage.Description.ByteOrder=riboLSBFirst then begin
      RedMemOffs:=RawImage.Description.RedShift div 8;
      GreenMemOffs:=RawImage.Description.GreenShift div 8;
      BlueMemOffs:=RawImage.Description.BlueShift div 8;
    end else begin
      RedMemOffs:=BytePerPix - 1 - (RawImage.Description.RedShift div 8);
      GreenMemOffs:=BytePerPix - 1 - (RawImage.Description.GreenShift div 8);
      BlueMemOffs:=BytePerPix - 1 - (RawImage.Description.BlueShift div 8);
    end;
    // populate the color row memory area for filling with bg color
    colorRowSize:=BytePerPix*Longint(RawImage.Description.Width);
    SetLength(colorRow, colorRowSize);
    for x:=0 to RawImage.Description.Width-1 do begin
      PixPtr:=PByte(colorRow) + x*BytePerPix;
      (PixPtr + RedMemOffs)^ := Byte(COLOR_UNDEFINED and $ff);
      (PixPtr + GreenMemOffs)^ := Byte((COLOR_UNDEFINED shr 8) and $ff);
      (PixPtr + BlueMemOffs)^ := Byte((COLOR_UNDEFINED shr 16) and $ff);
    end;
    // fill all rows with the background color row
    BytePtr:=DataPtr;
    for y:=0 to RawImage.Description.Height-1 do begin
      Move(PByte(colorRow)^, BytePtr^, colorRowSize);
      Inc(BytePtr, BytePerLine);
    end;
    // adjust colorrowsize to gridblock width
    colorRowSize:=BytePerPix*(Fgridsize-1);
    SetLength(colorRow, colorRowSize);
    numBlocks := Fhblocks*Fvblocks;
    for iImgBlock := 0 to numBlocks-1 do begin  // for all blocks...
      curBlockData := FGridBlockStore[iImgBlock];
      if (curBlockData and MASK_ACTIVE) <> 0 then actblock:=iImgBlock;
      // modify color if user is currently left-drag zooming on this block
      curBlockData := blendColors(COLOR_HIGHLIGHT, curBlockData and $ffffff,
        112*LongInt(not InRangeEx(iImgBlock, FBlockSelectStart, FBlockSelectEnd)));
      // populate the color row memory area with gridblock color
      PixPtr:=PByte(colorRow);
      for x:=0 to Fgridsize-2 do begin // the pixels of a gridblock-sized line
        (PixPtr + RedMemOffs)^ := Byte(curBlockData and $ff);
        (PixPtr + GreenMemOffs)^ := Byte((curBlockData shr 8) and $ff);
        (PixPtr + BlueMemOffs)^ := Byte((curBlockData shr 16) and $ff);
        Inc(PixPtr, BytePerPix);
      end;
      vblock := iImgBlock div Fhblocks; // vertical index of current block
      hblock := iImgBlock mod Fhblocks; // horizontal index of current block
      ys:=vblock*Fgridsize+1; // y start line of gridblock color area in image
      ye:=ys+Fgridsize-2;     // y last/ending line ...
      xs:=hblock*Fgridsize+1; // x start column of gridblock color area
      BytePtr:=DataPtr+ys*BytePerLine+xs*BytePerPix; // start of gridblock in image data
      for y:=ys to ye do begin
        Move(PByte(colorRow)^, BytePtr^, colorRowSize); // copy row to img memory
        Inc(BytePtr, BytePerLine);  // increase by one row
      end;
    end;
  finally
    BlockImage.Picture.Bitmap.EndUpdate(false);
  end;
  with BlockImage.Canvas do begin // active border frame, in canvas mode
    if actblock <> -1 then begin
      Brush.Color := COLOR_ACTIVE;
      FrameRect(resizeRect(getImgBlockRect(actblock), 1));
    end;
    if FSelectedBlock >= 0 then begin  // selected block frame, in canvas mode
      Brush.Color := COLOR_SELECTED;
      FrameRect(resizeRect(getImgBlockRect(FSelectedBlock), 2));
    end;
  end;
  BlockImage.Refresh;
  //DbgLog.Lines.Add('Grid blocks drawn: '+inttostr(FGridBlockStore.NumBlocks));
end;

// draw stuff in the top bar.
procedure TMainForm.drawTopBarItems;
var
  angle, alength: extended;
begin
  //Example blocks in the top bar
  ShapeFinished.Brush.Color:=COLOR_FINISHED;
  ShapeNonTried.Brush.Color:=COLOR_NON_TRIED;
  ShapeBad.Brush.Color:=COLOR_BAD_SECT;
  ShapeNonScraped.Brush.Color:=COLOR_NON_SCRAPED;
  ShapeNonTrimmed.Brush.Color:=COLOR_NON_TRIMMED;
  ShapeActive.Pen.Color:=COLOR_ACTIVE;
  ShapeOutsideDomain.Brush.Color:=COLOR_OUTSIDE_DOMAIN;

  // Build Pie image
  PieImage.Canvas.Brush.Color := TopPiePanel.Color;
  PieImage.Canvas.FillRect(PieImage.Canvas.ClipRect);
  with FGridBlockStore.Parser do begin
    if hasFile() and (rescueStatus.devicesize <> 0) then begin
      PieImage.Canvas.Brush.Color := COLOR_FINISHED;
      alength:=rescueStatus.rescued/rescueStatus.devicesize*5760;
      PieImage.Canvas.RadialPie(1, 1, 63, 63, 0, round(alength));
      angle:=alength;

      PieImage.Canvas.Brush.Color := COLOR_NON_TRIED;
      alength:=rescueStatus.nontried/rescueStatus.devicesize*5760;
      PieImage.Canvas.RadialPie(1, 1, 63, 63, round(angle), round(alength));
      angle:=angle+alength;

      PieImage.Canvas.Brush.Color := COLOR_BAD_SECT;
      alength:=rescueStatus.bad/rescueStatus.devicesize*5760;
      PieImage.Canvas.RadialPie(1, 1, 63, 63, round(angle), round(alength));
      angle:=angle+alength;

      PieImage.Canvas.Brush.Color := COLOR_NON_SCRAPED;
      alength:=rescueStatus.nonscraped/rescueStatus.devicesize*5760;
      PieImage.Canvas.RadialPie(1, 1, 63, 63, round(angle), round(alength));
      angle:=angle+alength;

      PieImage.Canvas.Brush.Color := COLOR_NON_TRIMMED;
      alength:=rescueStatus.nontrimmed/rescueStatus.devicesize*5760;
      PieImage.Canvas.RadialPie(1, 1, 63, 63, round(angle), round(alength));
      angle:=angle+alength;

      PieImage.Canvas.Brush.Color := COLOR_OUTSIDE_DOMAIN;
      alength:=rescueStatus.outsidedomain/rescueStatus.devicesize*5760;
      PieImage.Canvas.RadialPie(1, 1, 63, 63, round(angle), round(alength));
    end else PieImage.Canvas.Ellipse(1, 1, 63, 63);
  end;
end;


// draw the left-hand side zooming bar using the blocks in FZoomBlockStore
procedure TMainForm.drawZoomImage;
  // Horz. or vert. dashed line. Assumes x1<=x2 and y1<=y2. Canvas bug workaround.
  procedure DashLine(canvas:TCanvas; x1, y1, x2, y2:integer);
  const DASHL=3; GAPL=4;
  var
    x,y,xd,yd,sgnx, sgny: integer;
  begin
    x:=x1; y:=y1;
    sgnx:=Sign(x2-x1);
    sgny:=Sign(y2-y1);
    while true do begin
      xd:=EnsureRange(x+sgnx*DASHL, x1, x2);
      yd:=EnsureRange(y+sgny*DASHL, y1, y2);
      canvas.Line(x, y, xd, yd);
      x+=sgnx*(DASHL+GAPL); y+=sgny*(DASHL+GAPL);
      if ((xd=x2) and (yd=y2)) or ((x>=x2) and (y>=y2)) then break;
    end;
  end;

var
  iImgBlock, zoomWndFirstPx, zoomWndLastPx, cPosBlock : Longint;
begin
  if FZoomBlockStore.Parser.hasFile() then begin
    if FZoomBarSelectStart <> -1 then begin
      zoomWndFirstPx:=min(FZoomBarSelectStart, FZoomBarSelectEnd);
      zoomWndLastPx:=max(FZoomBarSelectStart, FZoomBarSelectEnd);
    end else begin
      zoomWndFirstPx:=FZoomBlockStore.getBlockFromOffset(FGridBlockStore.SectSize*
          FGridBlockStore.SectOffset);
      zoomWndLastPx:=FZoomBlockStore.getBlockFromOffset(FGridBlockStore.SectSize*
        (FGridBlockStore.SectOffset+FGridBlockStore.SectCount)-1);
    end;
  end else begin
    zoomWndFirstPx:=0;
    zoomWndLastPx:=ZoomImage.Height-1;
  end;

  with ZoomImage.Canvas do begin
    Line(0, 0, Width, 0); // prevents a canvas glitch
    Pen.EndCap:=pecFlat;

    // Draw the block colors
    for iImgBlock := 0 to FZoomBlockStore.NumBlocks-1 do begin
      Pen.Color:=blendColors(COLOR_HIGHLIGHT, FZoomBlockStore[iImgBlock] and $ffffff,
        112*LongInt(not InRange(iImgBlock, zoomWndFirstPx, zoomWndLastPx)));
      Line(0, iImgBlock, Width, iImgBlock);
    end;

    if FZoomBlockStore.Parser.hasFile() then begin
      // Draw the zoom frame
      Pen.Color:=$ff9933;
      DashLine(ZoomImage.Canvas, 0, zoomWndFirstPx, Width, zoomWndFirstPx); // top border
      DashLine(ZoomImage.Canvas, 0, zoomWndLastPx, Width, zoomWndLastPx);   // bottom border
      Pen.Width:=3;          // thick line style
      Line(1, zoomWndFirstPx, 1, zoomWndLastPx+1);  // left border
      Line(Width-2, zoomWndFirstPx, Width-2, zoomWndLastPx+1); // right border

      cPosBlock:=FZoomBlockStore.getBlockFromOffset(FZoomBlockStore.Parser.rescueStatus.pos);

      // Draw the current position marker
      Pen.Width:=5;
      Pen.Color:=clBlack;
      Line(0,cPosBlock, 4, cPosBlock);
      Line(Width-4,cPosBlock, Width, cPosBlock);
      Pen.Width:=3;
      Line(4,cPosBlock, 5, cPosBlock);
      Line(Width-5,cPosBlock, Width-4, cPosBlock);
      Pen.Width:=1;
      Line(5,cPosBlock, 6, cPosBlock);
      Line(Width-6,cPosBlock, Width-5, cPosBlock);
      Pen.Color := COLOR_ACTIVE;
      Pen.Width:=3;
      Line(0,cPosBlock, 4, cPosBlock);
      Line(Width-4,cPosBlock, Width, cPosBlock);
      Pen.Width:=1;
      Line(0,cPosBlock, 5, cPosBlock);
      Line(Width-5,cPosBlock, Width, cPosBlock);
    end;
  end;
  ZoomImage.Refresh;
  //DbgLog.Lines.Add('ZoomBar Lines drawn: '+inttostr(FZoomBlockStore.NumBlocks));
end;

// select an img block from the block grid. This happens when a block is clicked.
procedure TMainForm.SelectBlock(block : integer);
begin
  if FSelectedBlock<>block then begin
    FSelectedBlock:=block;
    if Assigned(BlockForm) then BlockForm.Block:=block;
    drawBlocks;
  end;
end;

// thats a long funtion name. Returns grid block number if allowed, -1 otherwise.
function TMainForm.BlockImageMouseActionAllowedGetBlock(X, Y: Integer) : Longint;
var imgBlock: Longint;
begin
  result := -1;
  imgBlock := GridXYToBlock(X, Y); // calculate block number
  if not FGridBlockStore.Parser.hasFile() then exit;  // empty map, no file opened
  if FGridBlockStore.getOffsetFromBlock(imgBlock) >=
      FGridBlockStore.Parser.rescueStatus.devicesize then exit; // not in device
  result := imgBlock;
end;

// user clicked the Block Grid
procedure TMainForm.BlockImageMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var imgBlock: Longint;
begin
  imgBlock:=BlockImageMouseActionAllowedGetBlock(X, Y);
  if imgBlock = -1 then exit; // mouse action not allowed
  if FBlockSelectStart <> -1 then exit; // no additional actions allowed

  if (Button = mbLeft) and Assigned(BlockForm) then begin
    SelectBlock(imgBlock);
    // Block Inspector initialization
    if not BlockForm.Visible then BlockForm.Show;
  end else if (Button = mbRight) and not miCenterCurPos.Checked then begin
    TMouseCapImage(BlockImage).MouseCapture:=true;
    FBlockSelectStart:=imgBlock;
    FBlockSelectEnd:=imgBlock;
    drawBlocks;
  end;
end;

procedure TMainForm.BlockImageMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
var imgBlock: Longint;
begin
  if FBlockSelectStart = -1 then exit; // must be in zooming mode
  imgBlock:=BlockImageMouseActionAllowedGetBlock(EnsureRange(X, 0, BlockImage.Width-2),
    EnsureRange(Y, 0, BlockImage.Height-2));
  if imgBlock = -1 then exit; // mouse action not allowed
  if imgBlock<>FBlockSelectEnd then begin
    FBlockSelectEnd:=imgBlock;
    drawBlocks;
  end;
end;

procedure TMainForm.BlockImageMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var StartBlock, EndBlock : Longint;
begin
  if Button = mbRight then begin
    if FBlockSelectStart = -1 then exit; // must be in zooming mode
    TMouseCapImage(BlockImage).MouseCapture:=false;
    StartBlock:=min(FBlockSelectStart, FBlockSelectEnd);
    EndBlock:=max(FBlockSelectStart, FBlockSelectEnd);
    FBlockSelectStart:=-1; FBlockSelectEnd:=FhBlocks*FvBlocks;
    drawBlocks;
    FGridBlockStore.zoomInRange(StartBlock, EndBlock);
    drawZoomImage;
  end;
end;

// the user has scrolled the mouse wheel on the block grid -> perform zoom
procedure TMainForm.BlockImageMouseWheel(Sender: TObject; Shift: TShiftState;
  WheelDelta: Integer; MousePos: TPoint; var Handled: Boolean);
var
  imgBlock : Longint; // wheeled-on imgblock
begin
  imgBlock:=BlockImageMouseActionAllowedGetBlock(MousePos.x, MousePos.y);
  if imgBlock = -1 then exit;
  //SelectBlock(-1); // unselect block
  if WheelDelta>0 then begin // zoom in
    if FZoomBehaviour then FGridBlockStore.zoomBlockToBlock(imgBlock, imgBlock, 1/1.5)
    else FGridBlockStore.zoomToCenter(imgBlock, 1/1.5);
  end else if WheelDelta<0 then begin // zoom out
    if FZoomBehaviour then FGridBlockStore.zoomBlockToBlock(imgBlock, imgBlock, 1.5)
    else FGridBlockStore.zoomToCenter(imgBlock, 1.5);
  end;
  drawZoomImage;
end;

// mouse down on zoom bar, start dragging action if all requirements met.
procedure TMainForm.ZoomImageMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if FZoomBarSelectStart <> -1 then exit; // no additional actions allowed
  if miCenterCurPos.Checked then exit; // neither scrolling nor selecting makes sense
  if FZoomBlockStore.Parser.hasFile() then begin
    if (Button=mbLeft) and (ZoomImage.Cursor=crHandPoint) then begin
      FZoomBarScroll:=true;
      FZoomBarScrollOrigin:=Y;
    end else if Button=mbRight then begin
      if not (FZoomBlockStore.getOffsetFromBlock(Y) >=
      FZoomBlockStore.Parser.rescueStatus.devicesize) then begin
        TMouseCapImage(ZoomImage).MouseCapture:=true;
        FZoomBarSelectStart:=Y;
        FZoomBarSelectEnd:=Y;
        ZoomImageMouseMove(self, Shift, X, Y); // update immediately
      end;
    end;
  end;
end;

// mouse-over or dragging action on the zoomblock image
procedure TMainForm.ZoomImageMouseMove(Sender: TObject; Shift: TShiftState; X, Y: Integer);
var
  zoomWndFirstPx, zoomWndLastPx, YOffsetBefore, YOffsetAfter : Longint;
  zoomShift, adjustedShift, deviceSectors : Int64;  // all values in device sectors
  StartBlock, EndBlock : Longint;
  newSectOffset, newSectCount : Int64;
begin
  if FGridBlockStore.Parser.hasFile() then begin
    if FZoomBarScroll and (FZoomBarScrollOrigin<>Y) then begin
      // user dragged zoomed-in area, or 'window', of the device.
      with FGridBlockStore do if (SectInBlock*FhBlocks) <> 0 then begin
        // calculate how much the user tries to shift the window (in sectors).
        // this can be way outside the zoom panel -> outside the device!
        zoomShift:=FZoomBlockStore.SectInBlock*Int64(Y-FZoomBarScrollOrigin);
        // adjust the shift, so it is aligned to a multiple of one grid line.
        // this eases the scrolling on the eyes, the blocks will not jump erratically.
        adjustedShift:=(zoomShift div (SectInBlock*FhBlocks)) * SectInBlock*FhBlocks;
        // get device size, in sectors
        deviceSectors:=Parser.rescueStatus.devicesize div SectSize;
        // save the current window offset, in pixels on the zoom bar.
        YOffsetBefore:=FZoomBlockStore.getBlockFromOffset(SectSize*SectOffset);
        // set the new window offset. The window is adjusted to the device bounds.
        SectOffset:=max(0, min(SectOffset+adjustedShift, deviceSectors-SectCount));
        // find out by how many pixels the window was actually shifted, after adjustment.
        YOffsetAfter:=FZoomBlockStore.getBlockFromOffset(SectSize*SectOffset);
        FZoomBarScrollOrigin+=YOffsetAfter-YOffsetBefore;
      end;
      FGridBlockStore.FPOObservedChanged(self, ooChange, Pointer(0));
      drawZoomImage;
    end else if FZoomBarSelectStart <> -1 then begin
      Y:=EnsureRange(Y, 0, ZoomImage.Height-1);
      if ((Y<>FZoomBarSelectEnd) or (FZoomBarSelectStart=FZoomBarSelectEnd))
      and (FZoomBlockStore.getOffsetFromBlock(Y) <
      FZoomBlockStore.Parser.rescueStatus.devicesize) then begin
        FZoomBarSelectEnd:=Y;
        StartBlock:=min(FZoomBarSelectStart, FZoomBarSelectEnd);
        EndBlock:=max(FZoomBarSelectStart, FZoomBarSelectEnd);
        newSectOffset := FZoomBlockStore.getOffsetFromBlock(StartBlock) div
          FZoomBlockStore.SectSize;
        newSectCount := FZoomBlockStore.getOffsetFromBlock(EndBlock) div
          FZoomBlockStore.SectSize - newSectOffset + FZoomBlockStore.SectInBlock;
        if FZoomBarSelectEnd < FZoomBlockStore.NumBlocks-1 then
          newSectCount := max(1, newSectCount div FGridBlockStore.NumBlocks *
          FGridBlockStore.NumBlocks)
        else newSectCount := min(newSectCount,
          FZoomBlockStore.Parser.rescueStatus.devicesize div FZoomBlockStore.SectSize -
          newSectOffset);
        FGridBlockStore.setZoom(newSectOffset, newSectCount);
        FGridBlockStore.FPOObservedChanged(self, ooChange, Pointer(0));
        drawZoomImage;
      end;
    end;

    // select the correct mouse cursor
    zoomWndFirstPx:=FZoomBlockStore.getBlockFromOffset(FGridBlockStore.SectSize *
      FGridBlockStore.SectOffset);
    zoomWndLastPx:=FZoomBlockStore.getBlockFromOffset(FGridBlockStore.SectSize *
      (FGridBlockStore.SectOffset+FGridBlockStore.SectCount));
    if InRange(Y, zoomWndFirstPx, zoomWndLastPx) and
    (FZoomBarSelectStart = -1) and not miCenterCurPos.Checked then
      ZoomImage.Cursor:=crHandPoint
      else ZoomImage.Cursor:=crDefault;
  end;
end;

procedure TMainForm.ZoomImageMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbLeft then FZoomBarScroll:=false
  else if (Button = mbRight) and (FZoomBarSelectStart <> -1) then begin
    TMouseCapImage(ZoomImage).MouseCapture:=false;
    FZoomBarSelectStart:=-1; FZoomBarSelectEnd:=ZoomImage.Height;
    //drawZoomImage;
  end;
end;

// user wants to open a file (file open dialog)
procedure TMainForm.miOpenMapfileClick(Sender: TObject);
begin
  OpenDialog1.Title:='Open ddrescue mapfile:';
  if OpenDialog1.Execute then begin
    OpenFile(OpenDialog1.FileName);
  end;
end;

// user opens a domain mapfile, in addition to the rescue mapfile.
// Todo: notify in the statusbar about opening progress
procedure TMainForm.miOpenDomFileClick(Sender: TObject);
begin
  OpenDialog1.Title:='Open ddrescue domain mapfile:';
  if OpenDialog1.Execute then OpenDomainFile(OpenDialog1.FileName);
end;

// user wants to close current file
procedure TMainForm.miCloseMapfileClick(Sender: TObject);
begin
  BlockForm.Hide;
  FParser.CloseFile;
end;

// user toggles option 'Center on Current position'
procedure TMainForm.miCenterCurPosClick(Sender: TObject);
begin
  GridBlockStore.LockOnCurPos:=miCenterCurPos.Checked;
  drawZoomImage;
end;

// user wants to show the bottom panel (debug log and mapfile comments)
procedure TMainForm.miShowMemosClick(Sender: TObject);
begin
  BottomPanel.Visible:=not BottomPanel.Visible;
  miShowMemos.Checked:=BottomPanel.Visible;
  MainVSplitter.Visible:=BottomPanel.Visible;
  // workaround: have the splitter not slide below the BottomPanel
  if MainVSplitter.Visible then begin
    MainVSplitter.Top:=0;
    FormResize(self);
    GridPanelResize(self);
  end;
end;


// user wants to toggle the zoom bar
procedure TMainForm.miZoomBarClick(Sender: TObject);
begin
  ZoomPanel.Visible:=not ZoomPanel.Visible;
  miZoomBar.Checked:=ZoomPanel.Visible;
  GridPanelResize(self);
end;

// take a snapshot the form and show a save dialog (save as png)
procedure TMainForm.saveFormImageClick(Sender: TObject);
var
  formBitmap: TBitmap = nil;
  MyPicture: TPicture = nil;
begin
  try
    if SaveDialog1.Execute then begin
      Application.ProcessMessages; //allow mainform to update after dialog close
      formBitmap := GetFormImage;
      formBitmap.Canvas.Changed;
      MyPicture := TPicture.Create;
      MyPicture.Bitmap:=formBitmap;
      MyPicture.SaveToFile(SaveDialog1.FileName, 'png');
    end;
  except
    on e: Exception do begin
      DbgLog.Lines.Add(e.Message);
    end;
  end;
  FreeAndNil(formBitmap);
  FreeAndNil(MyPicture);
end;


// user clicked exit from the menu
procedure TMainForm.miExitClick(Sender: TObject);
begin
  BlockForm.Hide;
  SettingsForm.Hide;
  Application.Terminate;
end;

// refresh requested, either from the menu or programmatically
procedure TMainForm.miRefreshClick(Sender: TObject);
begin
  if Sender = RetryTimer then begin
    DbgLog.Lines.Add('Retrying to refresh mapfile.');
    RetryTimer.Enabled := false;
  end;
  StatusBar.Panels.Items[0].Text:=buildStatusBarText(FParser.MapFileName, true, false);
  StatusBar.Refresh;
  Application.ProcessMessages;
  if not FParser.parse(true) then begin
    // The rescue device size has changed. This is most likely caused by
    // ddrescue currently writing to the mapfile. The Information in the Parser
    // has not been changed, so we need to retry parsing in 100ms.
    DbgLog.Lines.Add('Incomplete mapfile on refresh! Retrying in 100ms...');
    RetryTimer.Enabled := true;
  end;
end;

// user clicked the little (x) button on the zoom bar.
procedure TMainForm.BtnCloseZoomBarClick(Sender: TObject);
begin
  miZoomBarClick(self);
end;

procedure TMainForm.DbgLogDblClick(Sender: TObject);
begin
  if MessageDlg('Debug log', 'Clear log?', mtConfirmation, [mbYes, mbNo], 0) = mrYes then DbgLog.Clear;
end;

// user chooses whether to show the domain mapfile - assisted rescue map
// in the default or in the contiguous fashion.
procedure TMainForm.miContigDomClick(Sender: TObject);
begin
  FParser.ContiguousDomain:=miContigDom.Checked;
end;

// user double-clicked a label in the rescue stat box.
// Toggle the stat display modes.
procedure TMainForm.rsLabelClick(Sender: TObject);
begin
  if Sender is TLabel then begin
    (Sender as TLabel).Tag:=((Sender as TLabel).Tag+1) mod 3;
    updateStatusTexts;
  end;
end;

// user wants to show the settings dialog.
procedure TMainForm.miSettingsClick(Sender: TObject);
begin
  SettingsForm.Show;
end;


// user selected a device sector size from the menu, --or--
// set FGridBlockStore.SectSize programatically, call DSClick to update menu
procedure TMainForm.DSClick(Sender: TObject);
var strSize, newCustomCap : string;
    itemID, i : Integer;
    newSize : Longint;
    doUpdate : Boolean = false;
    DSCustomChecked : Boolean = true; // initially select the custom entry
begin
  newSize:=FGridBlockStore.SectSize; // by default, use current size

  // determine the size from the clicked item
  if Sender is TMenuItem then begin
    doUpdate:=true;
    //TMenuItem(Sender).Checked:=true;
    if Sender = DSCustom then begin // user clicked "Custom size", show dialog
      strSize := InputBox('Custom sector size', 'Enter custom sector size in Bytes:',
        inttostr(FGridBlockStore.SectSize));
      newSize := strToIntDef(strSize, DEF_BSIZE);
    end else newSize:=TMenuItem(Sender).Tag; // size is stored in .Tag
  end else begin
    // depending on the size, determine which item has to be selected
    for itemID := 0 to miSectSize.Count-1 do begin
      if miSectSize.Items[itemID].Tag = newSize then begin
        if not miSectSize.Items[itemID].Checked then miSectSize.Items[itemID].Checked:=true;
        DSCustomChecked := false;
        break;
      end;
    end;
  end;
  if DSCustomChecked then newCustomCap := 'Custom ('+inttostr(newSize) + ') ...'
  else newCustomCap:= 'Custom...';

  // cannot use the properties directly, until they actually need to change,
  // to avoid menu bar flicker (on Windows, at least...)
  if DSCustom.Checked <> DSCustomChecked then DSCustom.Checked:=DSCustomChecked;
  if DSCustom.Caption <> newCustomCap then DSCustom.Caption:=newCustomCap;

  for i:=0 to Length(SFORMATS)-1 do (SFORMATS[i]^).SectSize:=newSize;
  if doUpdate then begin // will cause update
    FGridBlockStore.SectSize:=newSize;
    FZoomBlockStore.SectSize:=newSize;
  end;
end;

// user selected an automatic parse time interval from the menu
procedure TMainForm.autoParseClick(Sender: TObject);
var interval : integer;
begin
  RetryTimer.Enabled := false;
  if Sender is TMenuItem then begin
    TMenuItem(Sender).Checked := true;
    interval := TMenuItem(Sender).Tag * 1000;
    updateTimer.Enabled := interval <> 0;
    updateTimer.Interval := interval;
  end;
  miRefresh.Click;
end;


// user selected a unit system from the menu
procedure TMainForm.UnitFormatClick(Sender: TObject);
var i: Integer;
begin
  if Sender is TMenuItem then TMenuItem(Sender).Checked := true;
  useDecimalUnits := miDecimalPrefixes.Checked;
  if miDecimalPrefixes.Checked then
    for i:=0 to Length(SFORMATS)-1 do (SFORMATS[i]^).Prefix:=pDec
  else for i:=0 to Length(SFORMATS)-1 do (SFORMATS[i]^).Prefix:=pBin;
  FPONotifyObservers(self, ooChange, nil); // push change to all forms
end;

// user selected a grid size from the menu
procedure TMainForm.setGridSize(Sender: TObject);
begin
  if Sender is TMenuItem then begin
    TMenuItem(Sender).Checked := true;
    FgridSize:=TMenuItem(Sender).Tag; // size stored in tag field
    GridPanelResize(self); // image sizes depends on block size -> need to resize it.
  end;
end;

// shows the about box
procedure TMainForm.miAboutClick(Sender: TObject);
begin
  AboutBox.ShowModal;
  self.Show; // workaround: avoid mainform from losing focus after aboutbox closes
end;

// clears the debug log
procedure TMainForm.miClearlogClick(Sender: TObject);
begin
  DbgLog.Lines.Clear;
end;

// modify mouse-over Hint time on a string length basis
procedure TMainForm.ApplicationEvents1ShowHint(var HintStr: String; var CanShow: Boolean; var HintInfo: THintInfo);
begin
  HintInfo.HideTimeout:=2000+Length(HintStr)*100;
end;


end.

