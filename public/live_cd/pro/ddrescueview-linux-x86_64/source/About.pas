(*
   About.pas - About Box unit

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

unit About;

interface

uses Forms, StdCtrls, Buttons, ExtCtrls, Controls, Classes;

type

  { TAboutBox }

  TAboutBox = class(TForm)
    AboutPanel: TPanel;
    TopPanel: TPanel;
    ProgramIcon: TImage;
    ProgramName: TLabel;
    Version: TLabel;
    OKButton: TButton;
    AboutMemo: TMemo;
    desc: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  AboutBox: TAboutBox;

implementation
uses Shared, SysUtils;

{$R *.lfm}

{ TAboutBox }

procedure TAboutBox.FormCreate(Sender: TObject);
begin
  Version.Caption := Format('Version %s.%s.%s %s',
    [VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_SUFFIX]);
end;

procedure TAboutBox.FormShow(Sender: TObject);
begin
  Constraints.MinHeight:=Height+100-AboutMemo.Height;
  Constraints.MinWidth:=Width-(TopPanel.ClientWidth-(desc.Left+desc.Width));
end;

end.
 
