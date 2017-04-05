object FormOpenBLOB: TFormOpenBLOB
  Left = 0
  Top = 0
  Caption = #1054#1090#1082#1088#1099#1090#1080#1077' BLOB-'#1076#1072#1085#1085#1099#1093' '#1076#1083#1103' '#1079#1072#1087#1080#1089#1080' '#1074' '#1073#1072#1079#1091
  ClientHeight = 174
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBoxBlobParseMode: TGroupBox
    Left = 0
    Top = 0
    Width = 145
    Height = 174
    Align = alLeft
    Caption = #1056#1077#1078#1080#1084' '#1079#1072#1087#1080#1089#1080
    TabOrder = 0
    object CheckBoxBlobParseValueStorage: TCheckBox
      Left = 2
      Top = 32
      Width = 141
      Height = 17
      Align = alTop
      Ctl3D = True
      ParentCtl3D = False
      TabOrder = 0
      Visible = False
    end
    object CheckBoxBlobParseValue1C: TCheckBox
      Left = 2
      Top = 49
      Width = 141
      Height = 17
      Align = alTop
      TabOrder = 1
      Visible = False
    end
    object CheckBoxBlobParseUnzip: TCheckBox
      Left = 2
      Top = 66
      Width = 141
      Height = 17
      Align = alTop
      Caption = #1059#1087#1072#1082#1086#1074#1072#1090#1100' (Deflate)'
      TabOrder = 2
      Visible = False
    end
    object CheckBoxBlobParseUnzip2: TCheckBox
      Left = 2
      Top = 83
      Width = 141
      Height = 17
      Align = alTop
      Caption = #1059#1087#1072#1082#1086#1074#1072#1090#1100' 2 (Deflate 2)'
      TabOrder = 3
      Visible = False
    end
    object CheckBoxChangeRelatedFields: TCheckBox
      Left = 2
      Top = 15
      Width = 141
      Height = 17
      Align = alTop
      Caption = #1048#1079#1084#1077#1085#1103#1090#1100' '#1089#1074#1103#1079#1072#1085#1085#1099#1077' '#1087#1086#1083#1103
      TabOrder = 4
      Visible = False
    end
    object CheckBoxBlobParseContainer: TCheckBox
      Left = 2
      Top = 100
      Width = 141
      Height = 17
      Align = alTop
      Caption = #1050#1072#1082' '#1082#1086#1085#1090#1077#1081#1085#1077#1088
      TabOrder = 5
      Visible = False
      OnClick = CheckBoxBlobParseContainerClick
    end
    object CheckBoxBlobParseUsersUsr: TCheckBox
      Left = 2
      Top = 117
      Width = 141
      Height = 17
      Align = alTop
      Caption = #1064#1080#1092#1088#1086#1074#1072#1090#1100
      TabOrder = 6
      Visible = False
    end
  end
  object Panel1: TPanel
    Left = 145
    Top = 0
    Width = 490
    Height = 174
    Align = alClient
    BevelOuter = bvSpace
    TabOrder = 1
    object LabelCaption: TLabel
      Left = 1
      Top = 1
      Width = 488
      Height = 13
      Align = alTop
      Caption = #1060#1072#1081#1083
      ExplicitWidth = 26
    end
    object DirectoryEdit: TAdvDirectoryEdit
      Left = 1
      Top = 14
      Width = 488
      Height = 21
      EmptyTextStyle = []
      Flat = False
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -11
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Separator = ';'
      Align = alTop
      Color = clWindow
      ReadOnly = False
      TabOrder = 0
      Text = ''
      Visible = False
      OnChange = DirectoryEditChange
      Version = '1.3.3.4'
      ButtonStyle = bsButton
      ButtonWidth = 18
      Etched = False
      Glyph.Data = {
        CE000000424DCE0000000000000076000000280000000C0000000B0000000100
        0400000000005800000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F00000000FFF
        00000088888880FF00000B088888880F00000BB08888888000000BBB00000000
        00000BBBBBBB0B0F00000BBB00000B0F0000F000BBBBBB0F0000FF0BBBBBBB0F
        0000FF0BBB00000F0000FFF000FFFFFF0000}
      BrowseDialogText = 'Select Directory'
      ExplicitWidth = 489
    end
    object FileNameEdit: TAdvFileNameEdit
      Left = 1
      Top = 35
      Width = 488
      Height = 21
      EmptyTextStyle = []
      Flat = False
      LabelFont.Charset = DEFAULT_CHARSET
      LabelFont.Color = clWindowText
      LabelFont.Height = -11
      LabelFont.Name = 'Tahoma'
      LabelFont.Style = []
      Lookup.Separator = ';'
      Align = alTop
      Color = clWindow
      ReadOnly = False
      TabOrder = 1
      Text = ''
      Visible = False
      OnChange = FileNameEditChange
      Version = '1.3.3.4'
      ButtonStyle = bsButton
      ButtonWidth = 18
      Etched = False
      Glyph.Data = {
        CE000000424DCE0000000000000076000000280000000C0000000B0000000100
        0400000000005800000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00D00000000DDD
        00000077777770DD00000F077777770D00000FF07777777000000FFF00000000
        00000FFFFFFF0DDD00000FFF00000DDD0000D000DDDDD0000000DDDDDDDDDD00
        0000DDDDD0DDD0D00000DDDDDD000DDD0000}
      FilterIndex = 0
      DialogOptions = [ofFileMustExist, ofEnableSizing, ofDontAddToRecent]
      DialogKind = fdOpen
      ExplicitWidth = 489
    end
    object ButtonOK: TButton
      Left = 4
      Top = 62
      Width = 75
      Height = 25
      Caption = 'OK'
      ModalResult = 1
      TabOrder = 2
    end
    object Button1: TButton
      Left = 85
      Top = 62
      Width = 75
      Height = 25
      Caption = #1054#1090#1084#1077#1085#1072
      ModalResult = 2
      TabOrder = 3
    end
  end
end
