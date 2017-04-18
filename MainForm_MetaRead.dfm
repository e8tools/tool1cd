object FormMain: TFormMain
  Left = 580
  Top = 374
  Caption = 'MetaRead - test'
  ClientHeight = 402
  ClientWidth = 799
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  DesignSize = (
    799
    402)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControlMain: TPageControl
    Left = -2
    Top = 0
    Width = 801
    Height = 410
    ActivePage = TabSheetConfig
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object TabSheetTypes: TTabSheet
      Caption = #1057#1090#1072#1090#1080#1095#1077#1089#1082#1080#1077' '#1090#1080#1087#1099
      TabVisible = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        793
        382)
      object TreeViewTypes: TTreeView
        Left = 0
        Top = 28
        Width = 793
        Height = 354
        Anchors = [akLeft, akTop, akRight, akBottom]
        Indent = 19
        ReadOnly = True
        TabOrder = 0
      end
      object ButtonReadTypes: TButton
        Left = 0
        Top = -1
        Width = 145
        Height = 23
        Caption = #1055#1086#1082#1072#1079#1072#1090#1100' '#1090#1080#1087#1099
        TabOrder = 1
        OnClick = ButtonReadTypesClick
      end
    end
    object TabSheetConfig: TTabSheet
      Caption = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103
      ImageIndex = 1
      DesignSize = (
        793
        382)
      object ButtonLoadStorage: TButton
        Left = 704
        Top = 3
        Width = 88
        Height = 21
        Anchors = [akTop, akRight]
        Cancel = True
        Caption = #1056#1072#1079#1086#1073#1088#1072#1090#1100
        TabOrder = 2
        OnClick = ButtonLoadStorageClick
      end
      object PageControlConf: TPageControl
        Left = 0
        Top = 29
        Width = 796
        Height = 350
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        ActivePage = TabSheetConf
        Anchors = [akLeft, akTop, akRight, akBottom]
        TabOrder = 0
        object TabSheetConf: TTabSheet
          Caption = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103
          ImageIndex = 3
          object SplitterConf: TSplitter
            Left = 495
            Top = 0
            Width = 4
            Height = 322
            Align = alRight
            ExplicitLeft = 489
            ExplicitTop = -6
            ExplicitHeight = 294
          end
          object PageControlFileFormats: TPageControl
            Left = 499
            Top = 0
            Width = 289
            Height = 322
            ActivePage = TabSheetPicture
            Align = alRight
            TabOrder = 0
            object TabSheetText: TTabSheet
              Caption = #1058#1077#1082#1089#1090
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
              object Panel2: TPanel
                Left = 0
                Top = 0
                Width = 281
                Height = 294
                Align = alClient
                BevelOuter = bvNone
                TabOrder = 0
                object PanelViewIn1C: TPanel
                  Left = 0
                  Top = 0
                  Width = 281
                  Height = 25
                  Align = alTop
                  BevelOuter = bvNone
                  TabOrder = 0
                  Visible = False
                  object ButtonViewIn1C: TButton
                    Left = -1
                    Top = 0
                    Width = 73
                    Height = 25
                    Caption = #1055#1088#1086#1089#1084#1086#1090#1088
                    TabOrder = 0
                    OnClick = ButtonViewIn1CClick
                  end
                end
              end
            end
            object TabSheetPicture: TTabSheet
              Caption = #1050#1072#1088#1090#1080#1085#1082#1072
              ImageIndex = 1
              object ScrollBox1: TScrollBox
                Left = 0
                Top = 0
                Width = 281
                Height = 294
                Align = alClient
                BevelInner = bvNone
                BevelOuter = bvNone
                BorderStyle = bsNone
                Color = clWindow
                ParentColor = False
                TabOrder = 0
              end
            end
            object TabSheetHTML: TTabSheet
              Caption = 'HTML'
              ImageIndex = 2
              ExplicitLeft = 0
              ExplicitTop = 0
              ExplicitWidth = 0
              ExplicitHeight = 0
            end
          end
        end
        object TabSheetGeneratedTypes: TTabSheet
          Caption = #1058#1080#1087#1099' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
        object TabSheetMeta: TTabSheet
          Caption = #1054#1073#1098#1077#1082#1090#1099' '#1084#1077#1090#1072#1076#1072#1085#1085#1099#1093
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
        object TabSheetExport: TTabSheet
          Caption = #1042#1099#1075#1088#1091#1079#1082#1072
          ImageIndex = 3
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          DesignSize = (
            788
            322)
          object ButtonExport: TButton
            Left = 700
            Top = 3
            Width = 85
            Height = 21
            Anchors = [akTop, akRight]
            Caption = #1042#1099#1075#1088#1091#1079#1080#1090#1100
            TabOrder = 0
            OnClick = ButtonExportClick
          end
        end
      end
      object PanelStatusBar: TPanel
        Left = 0
        Top = 360
        Width = 793
        Height = 22
        Align = alBottom
        BevelOuter = bvLowered
        TabOrder = 1
        object PanelСurConfPath: TPanel
          Left = 1
          Top = 1
          Width = 637
          Height = 20
          Align = alClient
          Alignment = taLeftJustify
          BevelOuter = bvNone
          Caption = #169' awa 2014 - 2015'
          TabOrder = 0
        end
        object CheckBoxEnglish: TCheckBox
          Left = 638
          Top = 1
          Width = 81
          Height = 20
          Align = alRight
          Caption = #1040#1085#1075#1083#1080#1081#1089#1082#1080#1081
          TabOrder = 1
          OnClick = CheckBoxUseImagesClick
        end
        object CheckBoxUseImages: TCheckBox
          Left = 719
          Top = 1
          Width = 73
          Height = 20
          Align = alRight
          Caption = #1050#1072#1088#1090#1080#1085#1082#1080
          TabOrder = 2
          OnClick = CheckBoxUseImagesClick
        end
      end
    end
    object TabSheetLog: TTabSheet
      Caption = #1051#1086#1075
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        793
        382)
      object MemoLog: TMemo
        Left = 0
        Top = 0
        Width = 793
        Height = 379
        Anchors = [akLeft, akTop, akRight, akBottom]
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object TabSheetParam: TTabSheet
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object CheckBoxLogToFile: TCheckBox
        Left = 3
        Top = 2
        Width = 86
        Height = 17
        Caption = #1051#1086#1075' '#1074' '#1092#1072#1081#1083
        TabOrder = 0
        OnClick = CheckBoxLogToFileClick
      end
      object CheckBoxUseExternal: TCheckBox
        Left = 3
        Top = 25
        Width = 222
        Height = 17
        Caption = #1054#1090#1083#1086#1078#1077#1085#1085#1086#1077' '#1095#1090#1077#1085#1080#1077' '#1074#1085#1077#1096#1085#1080#1093' '#1092#1072#1081#1083#1086#1074
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = CheckBoxLogToFileClick
      end
    end
  end
end
