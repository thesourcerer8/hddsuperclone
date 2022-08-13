(*
   ddrescueview.lpr - main program of ddrescueview

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

program ddrescueview;

uses
  Forms, Interfaces,
  Shared in 'Shared.pas',
  Parser in 'Parser.pas',
  GUI in 'GUI.pas' {MainForm},
  BlockInspector in 'BlockInspector.pas' {BlockInspector},
  Settings in 'Settings.pas' {SettingsForm},
  About in 'About.pas' {AboutBox};

{$R *.res}

begin
  Application.Initialize;
  {$IFDEF WINDOWS}
  Application.{%H-}MainFormOnTaskbar := True;
  {$ENDIF}
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TBlockForm, BlockForm);
  Application.CreateForm(TSettingsForm, SettingsForm);
  Application.CreateForm(TAboutBox, AboutBox);
  Application.Run;
end.
