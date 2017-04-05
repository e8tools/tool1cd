object Form1: TForm1
  Left = 0
  Top = 0
  Caption = #1056#1072#1079'DT'
  ClientHeight = 66
  ClientWidth = 391
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    391
    66)
  PixelsPerInch = 96
  TextHeight = 13
  object AdvFileNameEditDT: TAdvFileNameEdit
    Left = 8
    Top = 8
    Width = 377
    Height = 21
    EmptyText = 'dt-'#1092#1072#1081#1083
    EmptyTextStyle = []
    Flat = False
    LabelFont.Charset = DEFAULT_CHARSET
    LabelFont.Color = clWindowText
    LabelFont.Height = -11
    LabelFont.Name = 'Tahoma'
    LabelFont.Style = []
    Lookup.Separator = ';'
    Anchors = [akLeft, akTop, akRight]
    Color = clWindow
    ReadOnly = False
    TabOrder = 0
    Text = ''
    Visible = True
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
    DialogOptions = [ofFileMustExist, ofEnableSizing]
    DialogTitle = #1042#1099#1073#1077#1088#1080#1090#1077' '#1092#1072#1081#1083' dt '#1080#1083#1080' '#1092#1072#1081#1083' '#1089' '#1076#1074#1086#1080#1095#1085#1099#1084' '#1076#1077#1088#1077#1074#1086#1084' 1'#1057
    DialogKind = fdOpen
  end
  object ButtonRazDT: TButton
    Left = 8
    Top = 35
    Width = 75
    Height = 25
    Caption = #1056#1072#1079'DT'#1100
    TabOrder = 1
    OnClick = ButtonRazDTClick
  end
end
