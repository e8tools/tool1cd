(* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is TurboPower Abbrevia
 *
 * The Initial Developer of the Original Code is
 * TurboPower Software
 *
 * Portions created by the Initial Developer are Copyright (C) 1997-2002
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** *)

{*********************************************************}
{* ABBREVIA: AbPeVer.pas                                 *}
{*********************************************************}
{* ABBREVIA: Property Editor - Version                   *}
{*   Use AbQPeVer.pas for CLX                            *}
{*********************************************************}

{$IFNDEF UsingCLX}
unit AbPeVer;
{$ENDIF}

{$I AbDefine.inc}

interface

uses
{$IFDEF MSWINDOWS}
  Windows,
  ShellAPI,
{$ENDIF}
{$IFDEF LibcAPI}
  Libc,
{$ENDIF}
{$IFDEF UsingClx}
  QGraphics,
  QForms,
  QControls,
  QStdCtrls,
  QButtons,
  QExtCtrls,
  QDialogs,
{$ELSE}
  Graphics,
  Forms,
  Controls,
  StdCtrls,
  Buttons,
  ExtCtrls,
  Dialogs,
{$ENDIF}
  DesignIntf,
  DesignEditors,
  SysUtils,
  Classes;

type
  TAbAboutBox = class(TForm)
    lblVersion: TLabel;
    Panel1: TPanel;
    Image1: TImage;
    btnOK: TButton;
    Panel2: TPanel;
    WebLbl: TLabel;
    NewsLbl: TLabel;
    Bevel1: TBevel;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Label10: TLabel;
    Label11: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure btnOKClick(Sender: TObject);
    procedure WebLblClick(Sender: TObject);
    procedure WebLblMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure NewsLblClick(Sender: TObject);
    procedure NewsLblMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure Panel2MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
  private
    { Private declarations }
  public
    { Public declarations }
  end;


  TAbVersionProperty = class( TStringProperty )
  public
    function GetAttributes: TPropertyAttributes;
             override;
    procedure Edit;
              override;
  end;

var
  AbAboutBox : TAbAboutBox;

implementation

{$IFNDEF UsingCLX}
{$R *.dfm}
{$ENDIF}

uses
  AbArcTyp,
  AbConst,
  AbResString;

{$IFDEF LINUX}
const
  { String Constants }
  sCannotStartBrowser = 'Unable to start web browser. Make sure you have it properly set-up on your system.';

const
  MaxBrowsers = 1;

type
  ECannotStartBrowser = class(Exception);

type
  TBrowserStartCmd = record
    Command    : string [64];
    Parameters : string [255];
    XTerm      : Boolean; { Start browser in an XTerm }
  end;

const
  { The list of browsers we can launch. }

  BrowserList : array [1..MaxBrowsers] of TBrowserStartCmd =
    ((Command : 'netscape'; Parameters : '<site>'; Xterm : False));


procedure GetCurrentPath (PathList : TStringList);
var
  WorkPath : PChar;
  StartPos : PChar;
  CurrentPath : PChar;
  State : (Scanning, GotColon);
begin
  WorkPath := getenv ('PATH');

  PathList.Clear;

  StartPos := WorkPath;
  State := Scanning;
  while (WorkPath^ <> #0) do begin
    case State of
      Scanning :
        begin
          if (WorkPath^ = ':') then begin
            State := GotColon;
            if (WorkPath <> StartPos) then begin
              CurrentPath := StrAlloc(WorkPath - StartPos + 1);
              StrLCopy(CurrentPath, StartPos, WorkPath-StartPos);
              PathList.Add (CurrentPath);
              StrDispose(CurrentPath);
            end;
          end;
        end;
      GotColon :
        begin
          if (WorkPath^ <> ':') then begin
            StartPos := WorkPath;
            State := Scanning;
          end;
        end;
    end;{case}
    inc(WorkPath);
  end;
  if (State = Scanning) and (WorkPath <> StartPos) then begin
    CurrentPath := StrAlloc(WorkPath - StartPos + 1);
    StrLCopy(CurrentPath, StartPos, WorkPath-StartPos);
    PathList.Add (CurrentPath);
    StrDispose(CurrentPath);
  end;
end;


function IsBrowserPresent (PathList : TStringList;
                                        Browser : string) : Boolean;
var
  i : integer;
begin
  Result := False;
  for i := 0 to PathList.Count - 1 do begin
    if FileExists (PathList[i] + '/' + Browser) then begin
      Result := True;
      exit;
    end;
  end;
end;

procedure CallBrowser (Browser    : string;
                                      Parameters : string;
                                      Website    : string;
                                      XTerm      : Boolean);
begin
  if Pos ('<site>', Parameters) > 0 then begin
    Parameters := Copy (Parameters, 1, Pos ('<site>', Parameters) - 1) +
                        Website +
                        Copy (Parameters, Pos ('<site>', Parameters) + 6, 255);
  end else
    Parameters := Parameters + ' ' + Website;
  if XTerm then begin
    Parameters := '-e ' + Browser + ' ' + Parameters;
    Browser := 'xterm';
  end;
  Libc.system (PChar (Browser + ' ' + Parameters + ' &'));
end;

procedure StartBrowser (Website : string);

var
  PathList : TStringList;
  i : integer;

begin
  PathList := TStringList.Create;
  try
    GetCurrentPath (PathList);
    for i := 1 to MaxBrowsers do begin
      if IsBrowserPresent (PathList, BrowserList[i].Command) then begin
        CallBrowser (BrowserList[i].Command, BrowserList[i].Parameters,
                     Website, BrowserList[i].XTerm);
        exit;
      end;
    end;
    raise ECannotStartBrowser.Create(sCannotStartBrowser);
  finally
    PathList.Free;
  end;
end;
{$ENDIF}


procedure TAbAboutBox.FormCreate(Sender: TObject);
begin
  Top := (Screen.Height - Height ) div 3;
  Left := (Screen.Width - Width ) div 2;
  lblVersion.Caption := Format(AbVersionFormatS, [AbVersionS] );
end;

function TAbVersionProperty.GetAttributes: TPropertyAttributes;
begin
  Result := [paDialog, paReadOnly];
end;

procedure TAbVersionProperty.Edit;
begin
  with TAbAboutBox.Create( Application ) do
    try
      ShowModal;
    finally
      Free;
    end;
end;

procedure TAbAboutBox.btnOKClick(Sender: TObject);
begin
  Close;
end;

procedure TAbAboutBox.WebLblClick(Sender: TObject);
begin
{$IFDEF MSWINDOWS }
  if ShellExecute(0, 'open', 'http://www.sourceforge.net/projects/tpabbrevia', '', '',
    SW_SHOWNORMAL) <= 32 then
    ShowMessage('Unable to start web browser');
{$ENDIF MSWINDOWS }
{$IFDEF LINUX }
  try
    StartBrowser('http://www.sourceforge.net/projects/tpabbrevia');
  except
    on ECannotStartBrowser do
      ShowMessage('Unable to start web browser');
  end;
{$ENDIF LINUX }
  WebLbl.Font.Color := clNavy;
end;

procedure TAbAboutBox.WebLblMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  WebLbl.Font.Color := clRed;
end;

procedure TAbAboutBox.NewsLblClick(Sender: TObject);
begin
{$IFDEF MSWINDOWS }
  if ShellExecute(0, 'open', 'http://www.sourceforge.net/forum/forum.php?forum_id=241865', '', '',
    SW_SHOWNORMAL) <= 32 then
    ShowMessage('Unable to start web browser');
{$ENDIF MSWINDOWS }
{$IFDEF LINUX }
  try
    StartBrowser('http://www.sourceforge.net/forum/forum.php?forum_id=241865');
  except
    on ECannotStartBrowser do
      ShowMessage('Unable to start web browser');
  end;
{$ENDIF LINUX }
  NewsLbl.Font.Color := clNavy;
end;

procedure TAbAboutBox.NewsLblMouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  NewsLbl.Font.Color := clRed;
end;

procedure TAbAboutBox.Panel2MouseMove(Sender: TObject; Shift: TShiftState;
  X, Y: Integer);
begin
  NewsLbl.Font.Color := clNavy;
end;

procedure TAbAboutBox.FormMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  WebLbl.Font.Color := clNavy;
  NewsLbl.Font.Color := clNavy;
end;

end.

