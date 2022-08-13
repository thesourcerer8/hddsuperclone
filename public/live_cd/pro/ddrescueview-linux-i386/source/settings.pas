(*
   Settings.pas - Settings GUI unit

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

unit Settings;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, ComCtrls,
  ExtCtrls, StdCtrls, Shared;

type

  { TSettingsForm }
  TSettingsForm = class(TForm, IAttachableObserved)
    btnClose: TButton;
    btnDefaults: TButton;
    cbFastDrawing: TCheckBox;
    cbAltZoom: TCheckBox;
    colBtnActive: TColorButton;
    colBtnOutsideDomain: TColorButton;
    colBtnSelected: TColorButton;
    colBtnBadSect: TColorButton;
    colBtnFinished: TColorButton;
    colBtnNonscraped: TColorButton;
    colBtnNontrimmed: TColorButton;
    EditBadSect: TLabeledEdit;
    EditFinished: TLabeledEdit;
    EditNonscraped: TLabeledEdit;
    EditOutsideDomain: TLabeledEdit;
    EditNontrimmed: TLabeledEdit;
    EditNontried: TLabeledEdit;
    lblActive: TLabel;
    lblSelected: TLabel;
    lblColor: TLabel;
    lblColoringWeight: TLabel;
    Panel1: TPanel;
    Panel10: TPanel;
    Panel11: TPanel;
    Panel2: TPanel;
    Panel3: TPanel;
    Panel4: TPanel;
    Panel5: TPanel;
    Panel6: TPanel;
    Panel7: TPanel;
    Panel8: TPanel;
    Panel9: TPanel;
    tbBadSect: TTrackBar;
    tbFinished: TTrackBar;
    tbNonscraped: TTrackBar;
    tbNontried: TTrackBar;
    colBtnNontried: TColorButton;
    tbOutsideDomain: TTrackBar;
    tbNontrimmed: TTrackBar;
    procedure btnCloseClick(Sender: TObject);
    procedure btnDefaultsClick(Sender: TObject);
    procedure cbAltZoomChange(Sender: TObject);
    procedure cbFastDrawingChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure somethingChanged(Sender: TObject);
  private
    { private declarations }
    FFreezeControls : boolean;
    procedure applySettings;
  public
    { public declarations }
    procedure AttachObserver(AObserver : TObject);
    procedure DetachObserver(AObserver : TObject);
  end;

var
  SettingsForm: TSettingsForm;

implementation
uses GUI;

{$R *.lfm}

{ TSettingsForm }

procedure TSettingsForm.AttachObserver(AObserver : TObject);
begin
  FPOAttachObserver(AObserver);
end;

procedure TSettingsForm.DetachObserver(AObserver : TObject);
begin
  FPODetachObserver(AObserver);
end;

procedure TSettingsForm.FormShow(Sender: TObject);
begin
  colBtnActive.Constraints.MinHeight:=colBtnBadSect.Height;
  colBtnSelected.Constraints.MinHeight:=colBtnBadSect.Height;

  FFreezeControls:=true;

  EditOutsideDomain.Text:=inttostr(WEIGHT_OUTSIDE_DOMAIN);
  tbOutsideDomain.Position:=WEIGHT_OUTSIDE_DOMAIN;
  colBtnOutsideDomain.ButtonColor:=TColor(COLOR_OUTSIDE_DOMAIN);

  EditNontried.Text:=inttostr(WEIGHT_NON_TRIED);
  tbNontried.Position:=WEIGHT_NON_TRIED;
  colBtnNontried.ButtonColor:=TColor(COLOR_NON_TRIED);

  EditNontrimmed.Text:=inttostr(WEIGHT_NON_TRIMMED);
  tbNontrimmed.Position:=WEIGHT_NON_TRIMMED;
  colBtnNontrimmed.ButtonColor:=TColor(COLOR_NON_TRIMMED);

  EditBadSect.Text:=inttostr(WEIGHT_BAD_SECT);
  tbBadSect.Position:=WEIGHT_BAD_SECT;
  colBtnBadSect.ButtonColor:=TColor(COLOR_BAD_SECT);

  EditFinished.Text:=inttostr(WEIGHT_FINISHED);
  tbFinished.Position:=WEIGHT_FINISHED;
  colBtnFinished.ButtonColor:=TColor(COLOR_FINISHED);

  EditNonscraped.Text:=inttostr(WEIGHT_NON_SCRAPED);
  tbNonscraped.Position:=WEIGHT_NON_SCRAPED;
  colBtnNonscraped.ButtonColor:=TColor(COLOR_NON_SCRAPED);

  colBtnActive.ButtonColor:=TColor(COLOR_ACTIVE);
  colBtnSelected.ButtonColor:=TColor(COLOR_SELECTED);

  FFreezeControls:=false;
end;

procedure TSettingsForm.FormCreate(Sender: TObject);
begin
  AttachObserver(MainForm.GridBlockStore);
  AttachObserver(MainForm.ZoomBlockStore);
end;

procedure TSettingsForm.btnCloseClick(Sender: TObject);
begin
  Hide;
end;

procedure TSettingsForm.btnDefaultsClick(Sender: TObject);
begin
  WEIGHT_OUTSIDE_DOMAIN:=DEF_WEIGHT_OUTSIDE_DOMAIN;
  WEIGHT_NON_TRIED:=DEF_WEIGHT_NON_TRIED;
  WEIGHT_FINISHED:=DEF_WEIGHT_FINISHED;
  WEIGHT_NON_SCRAPED:=DEF_WEIGHT_NON_SCRAPED;
  WEIGHT_NON_TRIMMED:=DEF_WEIGHT_NON_TRIMMED;
  WEIGHT_BAD_SECT:=DEF_WEIGHT_BAD_SECT;
  COLOR_OUTSIDE_DOMAIN:=DEF_COLOR_OUTSIDE_DOMAIN;
  COLOR_NON_TRIED:=DEF_COLOR_NON_TRIED;
  COLOR_FINISHED:=DEF_COLOR_FINISHED;
  COLOR_NON_SCRAPED:=DEF_COLOR_NON_SCRAPED;
  COLOR_NON_TRIMMED:=DEF_COLOR_NON_TRIMMED;
  COLOR_BAD_SECT:=DEF_COLOR_BAD_SECT;
  COLOR_ACTIVE:=DEF_COLOR_ACTIVE;
  COLOR_SELECTED:=DEF_COLOR_SELECTED;
  cbAltZoom.Checked:=true;
  cbFastDrawing.Checked:=true;
  FormShow(self);
  FPONotifyObservers(self, ooChange, nil);
end;

procedure TSettingsForm.cbAltZoomChange(Sender: TObject);
begin
  MainForm.setZoomBehaviour(cbAltZoom.Checked);
end;

procedure TSettingsForm.cbFastDrawingChange(Sender: TObject);
begin
  MainForm.setDrawFunction(false, Integer(cbFastDrawing.Checked)+1);
end;

procedure TSettingsForm.FormDestroy(Sender: TObject);
begin
  DetachObserver(MainForm.GridBlockStore);
  DetachObserver(MainForm.ZoomBlockStore);
end;

procedure TSettingsForm.somethingChanged(Sender: TObject);
begin
  if FFreezeControls then exit;
  applySettings;
  FormShow(self);
  FPONotifyObservers(self, ooChange, nil);
end;

procedure TSettingsForm.applySettings;
begin
  WEIGHT_OUTSIDE_DOMAIN:=tbOutsideDomain.Position;
  COLOR_OUTSIDE_DOMAIN:=colBtnOutsideDomain.ButtonColor;
  WEIGHT_NON_TRIED:=tbNontried.Position;
  COLOR_NON_TRIED:=colBtnNontried.ButtonColor;
  WEIGHT_NON_TRIMMED:=tbNontrimmed.Position;
  COLOR_NON_TRIMMED:=colBtnNontrimmed.ButtonColor;
  WEIGHT_BAD_SECT:=tbBadSect.Position;
  COLOR_BAD_SECT:=colBtnBadSect.ButtonColor;
  WEIGHT_FINISHED:=tbFinished.Position;
  COLOR_FINISHED:=colBtnFinished.ButtonColor;
  WEIGHT_NON_SCRAPED:=tbNonscraped.Position;
  COLOR_NON_SCRAPED:=colBtnNonscraped.ButtonColor;
  COLOR_ACTIVE:=colBtnActive.ButtonColor;
  COLOR_SELECTED:=colBtnSelected.ButtonColor;
end;

end.

