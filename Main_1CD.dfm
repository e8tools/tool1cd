object FormMain: TFormMain
  Left = 177
  Top = 186
  Anchors = [akLeft, akTop, akRight, akBottom]
  Caption = 'Tool_1CD'
  ClientHeight = 662
  ClientWidth = 870
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  OnActivate = FormActivate
  OnKeyDown = FormKeyDown
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter2: TSplitter
    Left = 0
    Top = 483
    Width = 870
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    ExplicitTop = 21
    ExplicitWidth = 482
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 643
    Width = 870
    Height = 19
    Constraints.MaxHeight = 19
    Constraints.MinHeight = 19
    Panels = <
      item
        Width = 300
      end>
  end
  object ButtonedEditFile: TButtonedEdit
    Left = 0
    Top = 0
    Width = 870
    Height = 21
    Align = alTop
    Images = ImageListMain
    LeftButton.DisabledImageIndex = 0
    LeftButton.HotImageIndex = 0
    LeftButton.ImageIndex = 8
    LeftButton.PressedImageIndex = 9
    LeftButton.Visible = True
    ReadOnly = True
    RightButton.Enabled = False
    TabOrder = 1
    OnLeftButtonClick = ActionFileOpenExecute
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 21
    Width = 870
    Height = 462
    ActivePage = TabSheetView
    Align = alClient
    Images = ImageListMain
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    object TabSheetTest: TTabSheet
      Caption = #1059#1090#1080#1083#1080#1090#1099
      ImageIndex = 7
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object PageControlUtils: TPageControl
        Left = 0
        Top = 0
        Width = 862
        Height = 453
        ActivePage = TabSheetStatistic
        Align = alClient
        TabOrder = 0
        object TabSheetConfigSave: TTabSheet
          Caption = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1081
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object ButtonSaveConfigSave: TButton
            Left = 3
            Top = 3
            Width = 222
            Height = 25
            Action = ActionSaveConfigSave
            TabOrder = 0
          end
          object ButtonSaveConfig: TButton
            Left = 3
            Top = 34
            Width = 222
            Height = 25
            Action = ActionSaveConfig
            TabOrder = 1
          end
          object PanelSupplierConfigs: TPanel
            Left = 0
            Top = 107
            Width = 854
            Height = 318
            Align = alBottom
            Anchors = [akLeft, akTop, akRight, akBottom]
            BevelOuter = bvNone
            TabOrder = 2
            Visible = False
            object sLabel1: TLabel
              Left = 3
              Top = 1
              Width = 138
              Height = 13
              Caption = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' '#1087#1086#1089#1090#1072#1074#1097#1080#1082#1072
            end
            object sPanel3: TPanel
              Left = 0
              Top = 285
              Width = 854
              Height = 33
              Align = alBottom
              BevelOuter = bvNone
              TabOrder = 0
              object sButton3: TButton
                Left = 3
                Top = 4
                Width = 222
                Height = 25
                Action = ActionFindSupplierConfigs
                TabOrder = 0
              end
              object sButton4: TButton
                Left = 231
                Top = 4
                Width = 222
                Height = 25
                Action = ActionSaveSupplierConfig
                TabOrder = 1
              end
            end
          end
        end
        object TabSheetDepotConfigSave: TTabSheet
          Caption = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1081
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object ButtonSaveLastDepotConfig: TButton
            Left = 3
            Top = 3
            Width = 278
            Height = 25
            Action = ActionSaveLastDepotConfig
            TabOrder = 0
          end
        end
        object TabSheetStatistic: TTabSheet
          Caption = #1060#1072#1081#1083#1099' '#1090#1072#1073#1083#1080#1094
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
      end
    end
    object TabSheetView: TTabSheet
      Margins.Left = 0
      Margins.Top = 0
      Margins.Right = 0
      Margins.Bottom = 0
      Caption = #1055#1088#1086#1089#1084#1086#1090#1088
      ImageIndex = 5
      object Splitter7: TSplitter
        Left = 185
        Top = 0
        Height = 433
        ExplicitLeft = 192
        ExplicitTop = 104
        ExplicitHeight = 100
      end
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 185
        Height = 433
        Align = alLeft
        BevelOuter = bvNone
        Constraints.MinWidth = 50
        TabOrder = 0
        object ListViewTables: TListView
          Left = 0
          Top = 31
          Width = 185
          Height = 402
          Align = alClient
          Color = clWhite
          Columns = <
            item
              AutoSize = True
            end>
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          HideSelection = False
          ReadOnly = True
          RowSelect = True
          ParentFont = False
          ShowColumnHeaders = False
          SmallImages = ImageListMain
          SortType = stData
          TabOrder = 0
          ViewStyle = vsReport
          OnChanging = ListViewTablesChanging
          OnCompare = ListViewTablesCompare
          OnSelectItem = ListViewTablesSelectItem
        end
        object Panel1: TPanel
          Left = 0
          Top = 0
          Width = 185
          Height = 31
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object Label1: TLabel
            Left = 1
            Top = 2
            Width = 45
            Height = 13
            Caption = #1058#1072#1073#1083#1080#1094#1099
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LabelCurrentTable: TLabel
            Left = 0
            Top = 18
            Width = 185
            Height = 13
            Align = alBottom
            ExplicitWidth = 3
          end
          object CheckBoxSortListTables: TCheckBox
            Left = 64
            Top = 1
            Width = 97
            Height = 16
            Caption = #1057#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100
            TabOrder = 0
            OnClick = CheckBoxSortListTables_Click
          end
        end
      end
      object PageControlTableView: TPageControl
        Left = 188
        Top = 0
        Width = 674
        Height = 433
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        ActivePage = TabSheetPhysicalView
        Align = alClient
        TabOrder = 1
        OnChange = PageControlTableViewChange
        object TabSheetDescription: TTabSheet
          Caption = #1054#1087#1080#1089#1072#1085#1080#1077
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object Splitter6: TSplitter
            Left = 432
            Top = 0
            Height = 425
            Align = alRight
            ExplicitLeft = 408
            ExplicitTop = 232
            ExplicitHeight = 100
          end
          object PanelDescriptionTable: TPanel
            Left = 435
            Top = 0
            Width = 231
            Height = 425
            Align = alRight
            BevelOuter = bvNone
            Constraints.MinWidth = 50
            TabOrder = 0
            object MemoDescriptionTable: TMemo
              Left = 0
              Top = 0
              Width = 231
              Height = 425
              Align = alClient
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              ScrollBars = ssBoth
              TabOrder = 0
            end
          end
          object Panel2: TPanel
            Left = 0
            Top = 0
            Width = 432
            Height = 425
            Align = alClient
            BevelOuter = bvNone
            Constraints.MinWidth = 50
            TabOrder = 1
            object Splitter1: TSplitter
              Left = 0
              Top = 105
              Width = 432
              Height = 3
              Cursor = crVSplit
              Align = alTop
              ExplicitTop = 108
              ExplicitWidth = 215
            end
            object Splitter5: TSplitter
              Left = 0
              Top = 323
              Width = 432
              Height = 3
              Cursor = crVSplit
              Align = alBottom
              ExplicitTop = 108
              ExplicitWidth = 215
            end
            object Panel5: TPanel
              Left = 0
              Top = 0
              Width = 432
              Height = 105
              Align = alTop
              BevelOuter = bvNone
              Constraints.MinHeight = 30
              TabOrder = 0
              object Label5: TLabel
                Left = 0
                Top = 0
                Width = 43
                Height = 13
                Align = alTop
                Caption = #1058#1072#1073#1083#1080#1094#1072
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Layout = tlCenter
              end
              object ValueListEditorDescriprionTable: TValueListEditor
                Left = 0
                Top = 13
                Width = 432
                Height = 92
                Align = alClient
                DefaultRowHeight = 16
                Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goAlwaysShowEditor, goThumbTracking]
                TabOrder = 0
                TitleCaptions.Strings = (
                  #1055#1072#1088#1072#1084#1077#1090#1088
                  #1047#1085#1072#1095#1077#1085#1080#1077)
                ColWidths = (
                  137
                  262)
                RowHeights = (
                  16
                  16)
              end
            end
            object Panel6: TPanel
              Left = 0
              Top = 326
              Width = 432
              Height = 99
              Align = alBottom
              BevelOuter = bvNone
              Constraints.MinHeight = 30
              TabOrder = 1
              object Label6: TLabel
                Left = 0
                Top = 0
                Width = 73
                Height = 19
                AutoSize = False
                Caption = #1048#1085#1076#1077#1082#1089#1099
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Layout = tlCenter
              end
              object TreeViewDescriptionIndexes: TTreeView
                Left = 0
                Top = 22
                Width = 432
                Height = 77
                Align = alBottom
                Anchors = [akLeft, akTop, akRight, akBottom]
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                Images = ImageListMain
                Indent = 19
                ParentFont = False
                ReadOnly = True
                TabOrder = 0
              end
              object sButton6: TButton
                Left = 95
                Top = 3
                Width = 116
                Height = 17
                Action = ActionDumpIndex
                TabOrder = 1
                Visible = False
              end
            end
            object Panel7: TPanel
              Left = 0
              Top = 108
              Width = 432
              Height = 195
              Align = alClient
              BevelOuter = bvNone
              Constraints.MinHeight = 30
              TabOrder = 2
              ExplicitHeight = 215
              object Label2: TLabel
                Left = 0
                Top = 0
                Width = 26
                Height = 13
                Align = alTop
                Caption = #1055#1086#1083#1103
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Layout = tlCenter
              end
            end
          end
        end
        object TabSheetPhysicalView: TTabSheet
          Caption = #1060#1080#1079#1080#1095#1077#1089#1082#1086#1077' '#1087#1088#1077#1076#1089#1090#1072#1074#1083#1077#1085#1080#1077
          ImageIndex = 1
          OnResize = TabSheetPhysicalViewResize
          object SplitterBlobView: TSplitter
            Left = 0
            Top = 185
            Width = 666
            Height = 3
            Cursor = crVSplit
            Align = alBottom
            ExplicitTop = 100
            ExplicitWidth = 639
          end
          object PanelBlobView: TPanel
            Left = 0
            Top = 188
            Width = 666
            Height = 217
            Align = alBottom
            BevelOuter = bvNone
            TabOrder = 0
            Visible = False
            object SplitterContainerList: TSplitter
              Left = 137
              Top = 0
              Height = 217
              ExplicitLeft = 136
              ExplicitTop = 80
              ExplicitHeight = 100
            end
            object SplitterBlobViewProperties: TSplitter
              Left = 512
              Top = 0
              Height = 217
              Align = alRight
              ExplicitLeft = 491
              ExplicitTop = 15
              ExplicitHeight = 301
            end
            object PanelBlobViewProperties: TPanel
              Left = 515
              Top = 0
              Width = 151
              Height = 217
              Align = alRight
              BevelOuter = bvNone
              Constraints.MinWidth = 80
              TabOrder = 0
              object GroupBoxBlobViewMode: TGroupBox
                Left = 0
                Top = 129
                Width = 151
                Height = 64
                Align = alTop
                Caption = #1056#1077#1078#1080#1084' '#1087#1088#1086#1089#1084#1086#1090#1088#1072
                TabOrder = 0
                DesignSize = (
                  151
                  64)
                object ComboBoxExBlobViewMode: TComboBoxEx
                  Left = 2
                  Top = 34
                  Width = 147
                  Height = 22
                  ItemsEx = <
                    item
                      Caption = 'ANSI'
                    end
                    item
                      Caption = 'UTF-8'
                    end
                    item
                      Caption = 'UTF-16'
                    end
                    item
                      Caption = 'Media'
                    end
                    item
                      Caption = 'WEB'
                    end
                    item
                      Caption = 'HEX'
                    end>
                  Style = csExDropDownList
                  Action = ActionBlobView
                  Anchors = [akLeft, akTop, akRight]
                  TabOrder = 0
                  OnChange = ComboBoxExBlobViewModeChange
                end
                object CheckBoxBlobViewModeAuto: TCheckBox
                  Left = 2
                  Top = 15
                  Width = 147
                  Height = 17
                  Action = ActionBlobViewModeDetect
                  Align = alTop
                  State = cbChecked
                  TabOrder = 1
                end
              end
              object GroupBoxBlobParseMode: TGroupBox
                Left = 0
                Top = 0
                Width = 151
                Height = 129
                Align = alTop
                Caption = #1056#1077#1078#1080#1084' '#1088#1072#1079#1073#1086#1088#1072
                TabOrder = 1
                object CheckBoxBlobParseValueStorage: TCheckBox
                  Left = 2
                  Top = 32
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseValueStorage
                  Align = alTop
                  Ctl3D = True
                  ParentCtl3D = False
                  TabOrder = 0
                end
                object CheckBoxBlobParseModeDetect: TCheckBox
                  Left = 2
                  Top = 15
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseModeDetect
                  Align = alTop
                  State = cbChecked
                  TabOrder = 1
                end
                object CheckBoxBlobParseValue1C: TCheckBox
                  Left = 2
                  Top = 49
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseValue1C
                  Align = alTop
                  TabOrder = 2
                end
                object CheckBoxBlobParseUnzip: TCheckBox
                  Left = 2
                  Top = 66
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseUnzip
                  Align = alTop
                  TabOrder = 3
                end
                object CheckBoxBlobParseUnzip2: TCheckBox
                  Left = 2
                  Top = 83
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseUnzip2
                  Align = alTop
                  TabOrder = 4
                end
                object CheckBoxBlobParseUsersUsr: TCheckBox
                  Left = 2
                  Top = 117
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseUsersUsr
                  Align = alTop
                  TabOrder = 5
                end
                object CheckBoxBlobParseContainer: TCheckBox
                  Left = 2
                  Top = 100
                  Width = 147
                  Height = 17
                  Action = ActionBlobParseContainer
                  Align = alTop
                  TabOrder = 6
                end
              end
              object CheckBoxBlobViewTextWrap: TCheckBox
                Left = 0
                Top = 193
                Width = 151
                Height = 17
                Action = ActionBlobViewTextWrap
                Align = alTop
                TabOrder = 2
              end
            end
            object PanelContainerList: TPanel
              Left = 0
              Top = 0
              Width = 137
              Height = 217
              Align = alLeft
              BevelOuter = bvNone
              Constraints.MinWidth = 80
              TabOrder = 1
              Visible = False
              object Label4: TLabel
                Left = 0
                Top = 0
                Width = 137
                Height = 13
                Align = alTop
                Caption = #1060#1072#1081#1083#1099' '#1082#1086#1085#1090#1077#1081#1085#1077#1088#1072
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Layout = tlCenter
                ExplicitWidth = 99
              end
              object TreeViewContainer: TTreeView
                Left = 0
                Top = 13
                Width = 137
                Height = 204
                Align = alClient
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                HideSelection = False
                Images = ImageListMain
                Indent = 19
                MultiSelectStyle = []
                ParentFont = False
                ReadOnly = True
                RowSelect = True
                TabOrder = 0
                OnChange = TreeViewContainerChange
                OnCollapsed = TreeViewContainerCollapsed
                OnExpanded = TreeViewContainerExpanded
              end
            end
          end
          object PanelPhysView: TPanel
            Left = 0
            Top = 0
            Width = 666
            Height = 185
            Align = alClient
            BevelOuter = bvNone
            Constraints.MinHeight = 100
            Ctl3D = False
            ParentCtl3D = False
            TabOrder = 1
            object ToolBarPhysView: TToolBar
              Left = 0
              Top = 0
              Width = 666
              Height = 24
              Caption = 'ToolBarPhysView'
              Color = clBtnFace
              Images = ImageListMain
              ParentColor = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 1
              object ToolButtonEditTable: TToolButton
                Left = 0
                Top = 0
                Action = ActionEditTable
              end
              object ToolButtonSaveTableChanges: TToolButton
                Left = 23
                Top = 0
                Action = ActionSaveTableChanges
              end
              object ToolButtonDiscardTableChanges: TToolButton
                Left = 46
                Top = 0
                Action = ActionDiscardTableChanges
              end
              object ToolButtonEditSeparator: TToolButton
                Left = 69
                Top = 0
                Width = 8
                Caption = 'ToolButtonEditSeparator'
                ImageIndex = 1
                Style = tbsSeparator
              end
              object ToolButtonEditTableInsert: TToolButton
                Left = 77
                Top = 0
                Action = ActionEditTableInsert
              end
              object ToolButtonEditTableDelete: TToolButton
                Left = 100
                Top = 0
                Action = ActionEditTableDelete
              end
              object ToolButtonInsDelSeparator: TToolButton
                Left = 123
                Top = 0
                Width = 8
                Caption = 'ToolButtonInsDelSeparator'
                ImageIndex = 0
                Style = tbsSeparator
              end
              object ComboBoxCurrentIndex: TComboBox
                Left = 131
                Top = 0
                Width = 177
                Height = 21
                Hint = #1058#1077#1082#1091#1097#1080#1081' '#1080#1085#1076#1077#1082#1089
                TabOrder = 0
                OnChange = ComboBoxCurrentIndexChange
              end
              object ToolButtonExportXMLSeparator: TToolButton
                Left = 308
                Top = 0
                Width = 8
                Caption = 'ToolButtonExportXMLSeparator'
                ImageIndex = 21
                Style = tbsSeparator
              end
              object ToolButtonExportTableToXML: TToolButton
                Left = 316
                Top = 0
                Action = ActionExportTableToXML
              end
              object ToolButtonSaveDepotConfig: TToolButton
                Left = 339
                Top = 0
                Action = ActionSaveDepotConfig
              end
              object ToolButton1: TToolButton
                Left = 362
                Top = 0
                Action = ActionSaveConfigExtension
              end
              object ToolButton2: TToolButton
                Left = 385
                Top = 0
                Action = ActionSaveConfigExtensionDb
              end
            end
            object PanelBlobTools: TPanel
              Left = 0
              Top = 163
              Width = 666
              Height = 22
              Align = alBottom
              BevelEdges = []
              BevelOuter = bvNone
              TabOrder = 0
              object LabelPhysPreview: TLabel
                Left = 92
                Top = 0
                Width = 574
                Height = 22
                Align = alClient
                AutoSize = False
                Layout = tlCenter
                ExplicitLeft = 131
                ExplicitTop = 3
                ExplicitWidth = 87
                ExplicitHeight = 13
              end
              object ToolBarBlobViewTools: TToolBar
                Left = 0
                Top = 0
                Width = 92
                Height = 22
                Align = alLeft
                AutoSize = True
                Caption = 'ToolBarBlobViewTools'
                Images = ImageListMain
                TabOrder = 0
                object ToolButtonWriteBlobToFile: TToolButton
                  Left = 0
                  Top = 0
                  Action = ActionWriteBlobToFile
                  Visible = False
                end
                object ToolButtonReadBlobFromFile: TToolButton
                  Left = 23
                  Top = 0
                  Action = ActionReadBlobFromFile
                  Visible = False
                end
                object ToolButtonBlobClear: TToolButton
                  Left = 46
                  Top = 0
                  Action = ActionBlobClear
                  Visible = False
                end
                object ToolButtonBlobRestore: TToolButton
                  Left = 69
                  Top = 0
                  Action = ActionBlobRestore
                  Visible = False
                end
              end
            end
          end
        end
        object TabSheetUtilities: TTabSheet
          Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
        end
      end
    end
  end
  object PanelMessages: TPanel
    Left = 0
    Top = 486
    Width = 870
    Height = 157
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Align = alBottom
    Caption = 'PanelMessages'
    Constraints.MinHeight = 30
    TabOrder = 3
    OnResize = PanelMessagesResize
    object Splitter3: TSplitter
      Left = 496
      Top = 1
      Height = 155
      Align = alRight
      ExplicitLeft = 640
      ExplicitTop = 40
      ExplicitHeight = 100
    end
    object ListViewMessage: TListView
      Left = 1
      Top = 1
      Width = 495
      Height = 155
      Align = alClient
      BevelEdges = []
      BevelInner = bvNone
      BevelOuter = bvNone
      Color = clWhite
      Columns = <
        item
          Width = 2000
        end>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      ParentFont = False
      ShowColumnHeaders = False
      StateImages = ImageListMessages
      TabOrder = 0
      ViewStyle = vsReport
      OnSelectItem = ListViewMessageSelectItem
    end
    object ValueListEditorMessage: TValueListEditor
      Left = 499
      Top = 1
      Width = 370
      Height = 155
      Align = alRight
      DefaultRowHeight = 16
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goEditing, goThumbTracking]
      TabOrder = 1
      TitleCaptions.Strings = (
        #1055#1072#1088#1072#1084#1077#1090#1088
        #1047#1085#1072#1095#1077#1085#1080#1077)
      ColWidths = (
        109
        255)
      RowHeights = (
        16
        16)
    end
  end
  object ImageListMessages: TImageList
    Left = 120
    Top = 584
    Bitmap = {
      494C010107001300940110001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000002000000001002000000000000020
      00000000000000000000000000000000000000000000EDEDED00D9D9D900CDCD
      CD00BEBEBE009E9E9E008A8A8A007B7B7B007B7B7B008A8A8A009E9E9E00BEBE
      BE00CDCDCD00D9D9D900EDEDED00000000000000000000000000FDFDFD00EFEF
      EF00DDDDDD00CFCFCF00C3C3C300464646002C2C2C00BDBDBD00CFCFCF00DBDB
      DB00EDEDED00FBFBFB0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F5F5F500E3E3E300A9A9
      A9008F8D8D00BDB1B100EBDADA00EBDADA00EBDADA00EBDADA00BDB1B1008F8D
      8D00A9A9A900E3E3E300F5F5F500000000000000000000000000FDFDFD00F7F7
      F700EFEFEF00E7E7E700ABABAB00C8C9C900A59F9F0067676700E7E7E700EDED
      ED00F5F5F500FDFDFD0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00F8F8F800B2B2B200A7A3
      A300E4D6D600DBCDCD00D2C4C400CABBBB00CABBBB00D2C4C400DBCDCD00E3D6
      D600A7A3A300B2B2B200F8F8F800FFFFFF000000000000000000000000000000
      00000000000000000000ABABAB00DCDDDD00BAAEAE0067676700000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00C4C4C400AAA7A700DBD1
      D100C7BBBB00D2C9C800EBE8E700F5F4F100F6F4F200EDE9E700D4CACA00C7BB
      BB00DACFCF00A9A6A600C4C4C400FFFFFF000000000000000000000000000000
      00000000000000000000ABABAB00B4B4B4008F8F8F0067676700000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EBEBEB00A3A2A200D4CECE00BCB3
      B300D4CFCA00F0F0E900F1F1EB00F1F1EB00F1F1EB00F1F1EB00F5F5F100DBD7
      D600BCB3B300D0C9C900A2A2A200EBEBEB000000000000000000000000000000
      000000000000F5F8FA007DAEBE00B2F1F100B1F1F1007DAEBE00F5F8FA000000
      0000000000000000000000000000000000000000000000000000000000000000
      000055899900C1D7DD0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C8C8C800B9B7B700BFBBBB00BDB9
      B500E3E3D400EFEFE700F1F1EB00DFDFDC00F1F1EB00F1F1EB00F1F1EB00F4F4
      EE00C3BEBD00BEB9B900B0AEAE00C8C8C8000000000000000000000000000000
      000000000000CFE0E70091BFCC00AFF0F000ADEFEF0091BFCC00CFE0E7000000
      0000000000000000000000000000000000000000000000000000000000007FB5
      C800D9F4FF00419EAF00BAD9DF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000B4B4B400D0CFCF00ACAAAA00D2D1
      C700DDDDCC00F1F1EA00DFDFDC0038383800DFDFDC00DFDFDC00DFDFDC00DFDF
      DB00D7D7D400ACAAAA00C3C1C100B4B4B4000000000000000000000000000000
      0000FDFDFE0091BAC800A2D3DB00A6EDED00A5EDED00A2D3DB0091B9C800FDFD
      FE0000000000000000000000000000000000000000000000000000000000D8EB
      F1004BB5C70079E6F700419EAF00BAD9DF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000ABABAB00CCCCCC009D9D9D00DCDC
      CD00DDDDCC00F1F1EA0035353500777777004848480055555500626262006E6E
      6E00B3B3B0009D9D9D00C0C0C000ABABAB000000000000000000000000000000
      0000B7D3DD008ABDCB00B2F0F10095E8E8009CEAEA00B9F3F30088B9C800B4D0
      DA00000000000000000000000000000000000000000000000000000000000000
      0000C1E8EE004BB5C70079E6F700419EAF00BAD9DF0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000AEAEAE00CFCFCF009D9E9E00DADA
      CB00DDDDCC00DDDDCC00F1F1EB004F4F4F00F1F1EB00F1F1EB00F1F1EB00EFEF
      E700E1E1D5009D9E9E00C3C3C300AEAEAE00000000000000000000000000DDEB
      F00087BACA00ABEAEC0072DCDC0071DDDD0080E1E10096E8E800BBEFF10083B3
      C300D9E7EC000000000000000000000000000000000000000000000000000000
      000000000000C1E8EE004BB5C70079E6F700419EAF00C0CFD200000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000BBBBBB00DBDCDC00ABACAC00CBCC
      BF00DDDDCC00DDDDCC00DDDDCC005C5C5C00DDDDCC00DDDDCC00DDDDCC00DDDD
      CC00D2D2C800ABACAC00CBCCCC00BBBBBB00000000000000000000000000A3CB
      D9009AD5DE0080E1E10064D8D8005FD7D70063D9D90078DEDE009CEAEA00A3D5
      DD0099C0CD000000000000000000000000000000000000000000000000000000
      00000000000000000000C1E8EE004BB5C70079E6F70093939300D4D4D4000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000CFCFCF00C9C9C900BFC0C000B5B5
      B000DDDDCC00E4E4D600F1F1EB0096969600F1F1EB00F1F1EB00F1F1EB00E6E6
      D800B7B8B400BDBFBF00BEBEBE00CFCFCF000000000000000000000000008DC0
      D200B6F0F10098E8E8009AE9E90087E3E30067D9D90064D8D80081E1E100BBF1
      F20081B3C5000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000C1E8EE00B2B2B200F7F7F7007E7E9700B8B8
      E400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EFEFEF00B7B7B700DFE0E000B6B7
      B700C5C5BC00DEDECD00EDEDE4009D9D9D00F1F1EB00F1F1EB00EDEDE400C6C7
      BF00B6B7B700D8D8D800B6B6B600EFEFEF0000000000000000000000000091C4
      D500C6F6F600BCF3F300BEF3F300B2F0F00087E3E3005FD7D70073DDDD00B7F2
      F20083B7C9000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E6E6E6009D9DB8008080FF003F3F
      C400000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00D3D3D300C0C0C000DFDF
      DF00BEBEBE00C0C0BB00D4D4C900C0C0BA00EDEDE700DFDFD900C2C2BE00BEBE
      BE00DBDCDC00BDBDBD00D3D3D300FFFFFF00000000000000000000000000A4D0
      DE00B6E4EC00C5F7F700C0F4F400BEF3F3009AE9E90064D8D80077DEDE00A9DF
      E6009BC6D5000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D0D0E9005151E800BFBF
      F000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FAFAFA00CBCBCB00C2C2
      C200E6E6E600D2D2D200C6C6C600BDBDBD00BDBDBD00C6C6C600D2D2D200E3E4
      E400C1C1C100CBCBCB00FAFAFA00FFFFFF00000000000000000000000000D5E9
      F0009DCEDD00CFF8F900C4F6F600BAF3F30094E7E7006EDBDB00A8ECED0095C9
      D700CFE4EB000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FAFAFA00D5D5
      D500BDBDBD00D1D1D100EAEAEA00E6E6E600E6E6E600EAEAEA00CFCFCF00BCBC
      BC00D5D5D500FAFAFA00FFFFFF00FFFFFF000000000000000000000000000000
      0000C1DFE9009FD0DF00B3E2EB00C0F3F400ACEDEE009EDCE40097CBDA00BDDC
      E600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00F1F1F100D5D5D500C5C5C500BDBDBD00BDBDBD00C5C5C500D5D5D500F1F1
      F100FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      000000000000D7EAF100AFD6E4009DCDDD009DCDDD00ABD4E100D7EAF0000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FBFB
      FB00F9F9F900F7F7F700F7F7F700F7F7F700F9F9F900FBFBFB00FDFDFD00FDFD
      FD000000000000000000000000000000000000000000FDFDFD00F3F3F300E9E9
      E900E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E9E9E900F3F3F300FDFDFD00000000000000000000000000FDFDFD00F1F1
      F100E7E7E700E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E7E7
      E700EFEFEF00FDFDFD00000000000000000000000000FDFDFD00F3F3F300E9E9
      E900E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E9E9E900F3F3F300FDFDFD00000000000000000000000000FBFBFB00EFEF
      EF00DDDDDD00B1BFB10089AA8900D1D3D100E1E1E100E7E7E700F1F1F100F7F7
      F700FDFDFD0000000000000000000000000000000000FBFBFB00E9E9E900D5D5
      D500818181003A3A3A003A3A3A003A3A3A003A3A3A003A3A3A003A3A3A008181
      8100D3D3D300E7E7E700FBFBFB00000000000000000000000000FBFBFB00E5E5
      E500CCC1BE00D0938400EC7D6100EE6C4B00EE6C4B00EC7D6100D0938400CCC1
      BE00E1E1E100F9F9F900000000000000000000000000FBFBFB00E9E9E900D5D5
      D500BBBBC1007E7EA000545490003939810039398100545490007E7EA000BBBB
      C100D3D3D300E7E7E700FBFBFB00000000000000000000000000FDFDFD00F9F9
      F900ABC6AB00429A3C0029C01D0080AD7F00F1F1F100F7F7F700F9F9F900FBFB
      FB00000000000000000000000000000000000000000000000000000000009B9B
      9B004848480027CBD90024DCEC0024DCEC0024DCEC0024DCEC0027CBD9004848
      48009B9B9B00000000000000000000000000000000000000000000000000E2BB
      B200F07A5C00F5A28F00FAD1CA00FCE7E500FCE6E500F9D1C900F4A18E00F079
      5B00E2BBB200000000000000000000000000000000000000000000000000B3B3
      CB00414196002323A7001717C6001212D5001212D5001717C6002323A7004141
      9600B3B3CB00000000000000000000000000000000000000000000000000CEE4
      CD0047A6400028CA180021CD110034A72B00BFD7BF0000000000000000000000
      0000000000000000000000000000000000000000000000000000A1A1A1005555
      550034CDD90028DBE80028DBE8007AEDF4007AEDF40028DBE80028DBE8002CCB
      D80055555500A1A1A10000000000000000000000000000000000F8C5B800EF79
      5C00F7D4CE00F7E9E900F7E8E800FCF4F400FCF4F400F7E8E800F7E9E900F6D1
      CA00EF795B00F8C5B80000000000000000000000000000000000B3B3D8003535
      9F001818C5001212B6001111D1001111D1001111D1001111D1001111B6001616
      C40034349F00B3B3D80000000000000000000000000000000000B8DBB4004FAD
      45002ACC190022C8110024C9130022C7110046A63F00C4DDC300000000000000
      00000000000000000000000000000000000000000000A5A5A5005959590048D0
      D8002DD9E3002DD9E3002DD9E30000000000000000002DD9E3002DD9E3002DD9
      E30030C9D30059595900A5A5A5000000000000000000FBEBE600E7755800F3D3
      CD00F0E5E500EFE4E400EFE4E400E5613F00E5613F00EFE4E400EFE4E400EFE4
      E400EFCDC700E7755800FBEBE6000000000000000000E5E5F2004242A7002020
      C2001212B300DCDCDC001111B2001111C8001111C8001111B200EEEEEE001111
      B2001616BE004141A600E5E5F2000000000000000000CFE8CD0054B2490047C0
      360025BF14002DC61C0044CD340026C1150025B5160055A94D00EFF6EF000000
      000000000000000000000000000000000000000000005D5D5D0060D3D80032D6
      DE0032D6DE0032D6DE0032D6DE00000000000000000032D6DE0032D6DE0032D6
      DE0032D6DE0035C8CF005D5D5D000000000000000000E9A79500E59D8A00EAE2
      E200E7DFDF00E7DFDF00E7DFDF00D6593700D6593700E7DFDF00E7DFDF00E7DF
      DF00E8E0E000E1978400E9A7950000000000000000009393CD002F2FB3001616
      C100D1D1D100D6D6D600DCDCDC001111AD001111AD00EAEAEA00EEEEEE00EEEE
      EE001111BE002424AC009393CD0000000000D1EACF0057B84B004FC63E0044BB
      330046C1360050B1450059B14F0048C13A0024B5130026AD160070B66A00EBF4
      EB0000000000000000000000000000000000000000006161610068E1E90038D4
      D90037D4D90037D4D90037D4D90081EAED0081EAED0037D4D90037D4D90037D4
      D90037D4D9003BD5DA00616161000000000000000000CF6F5300EAD0C800E1DA
      DA00E0D9D900E0D9D900E0D9D900C5502E00C5502E00E0D9D900E0D9D900E0D9
      D900E0D9D900E1C4BD00CF6F530000000000000000005858B4003636C2001212
      B4001111B400D1D1D100D6D6D600DCDCDC00E2E2E200E6E6E600EAEAEA001111
      B4001111B4001B1BB2005858B40000000000D9EED7005FBF530052C9410050C7
      3F0056B74A00B9DDB400BADEB70059B74E0045BF360024AC130033A625008FC7
      8A0000000000000000000000000000000000000000006565650083E7F30056D9
      E5003FD2D6003CD1D4003CD1D40000000000000000003CD1D4003CD1D4003CD1
      D4003CD1D40046D4D700656565000000000000000000B94F2E00F4EEED00E7E5
      E500DAD6D600D8D4D400D8D4D400B7482600B7482600D8D4D400D8D4D400D8D4
      D400D8D4D400E1D8D600B94F2E0000000000000000003939AA004545CD002626
      B5001414AC001111AA00D1D1D100D6D6D600DCDCDC00E2E2E2001111AA001111
      AA001111AA001919AF003939AA000000000000000000DDF0DB0061C1540059BD
      4C00D0EACD000000000000000000C6E4C30051B7440045B8350023A112002FA0
      2100AFD8AB00FBFDFB000000000000000000000000006969690088E9F40066DD
      EE0061DBEA004DD4DA0042CFD000000000000000000040CFCF0040CFCF0040CF
      CF0040CFCF0050D4D500696969000000000000000000B04B2A00F4EFED00EEEE
      EE00EAEAEA00DCDBDB00D3D1D100AD442200AD442200D2D0D000D2D0D000D2D0
      D000D2D0D000DFD8D700B04B2A0000000000000000003939AD004A4AD1003333
      BB002E2EB80013139F00CECECE00D1D1D100D6D6D600DCDCDC0011119E001111
      A1001111A1001E1EAC003939AD00000000000000000000000000D9EFD700D2EB
      CF00000000000000000000000000FDFEFD009AD2930057BF490042B8310033AA
      220052B14700C9E5C7000000000000000000000000006B6B6B008FECF60069DF
      F00069DFF00069DFF00063DCEB00303030001818180048CCD40045C9D20045C9
      D20048CCD40063D8DE006B6B6B000000000000000000BA674B00EAD5CD00F0F0
      F000F0F0F000F0F0F000EBEBEB00E9E9E900E7E6E600D2D1D100CECECE00CECE
      CE00D2D1D100DCC7C000BA674B0000000000000000005858BC004E4ED4003737
      BF002323AB0000000000F7F7F700E8E8E800DEDEDE00DBDBDB00DCDCDC001111
      9B001616A0002F2FB5005858BC00000000000000000000000000000000000000
      000000000000000000000000000000000000F7FBF70096D18F0057CB480057CE
      46004FC53F0057B84A00E0F1DF00000000000000000070707000B5EAED0070E2
      F30070E2F30070E2F30070E2F300666666006666660070E2F30070E2F30070E2
      F30070E2F30094E0E800707070000000000000000000D6A18F00D49D8A00F5F5
      F500F3F3F300F3F3F300F3F3F300B34F2D00B34F2D00F3F3F300F3F3F300F3F3
      F300F5F5F500D49C8900D6A18F0000000000000000009393D5004848CC004949
      D1000000000000000000000000004242CA004242CA0000000000000000000000
      00004747CF004141C4009393D500000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F1F9F10072C867005ED3
      4E005AD1490053C843007CC67300ECF6EB0000000000B2B2B20072727200BBEC
      EE0079E6F70079E6F70079E6F700757575007575750079E6F70079E6F70079E6
      F700A4E6EC0072727200B2B2B2000000000000000000F6EAE600C9725500F2DF
      D900F8F8F800F7F7F700F7F7F700C15D3B00C15D3B00F7F7F700F7F7F700F8F8
      F800F2DFD900C9725500F6EAE6000000000000000000E5E5F5004545BD005B5B
      E2005151D900000000004F4FD7004F4FD7004F4FD7004F4FD700000000005151
      D9005555DD004545BD00E5E5F500000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000E3F3E10071C8
      650067DD560067DE56005CC74F00CCE8C9000000000000000000B4B4B4007474
      7400BEEEEF0081EAFB0081EAFB0081EAFB0081EAFB0081EAFB0081EAFB00B1EA
      EE0074747400B4B4B40000000000000000000000000000000000ECC7BA00D782
      6400F6E3DC00FCFCFC00FBFBFB00FBFBFB00FBFBFB00FBFBFB00FCFCFC00F6E3
      DC00D7826400ECC7BA0000000000000000000000000000000000B3B3E4003E3E
      C0006060E8005F5FE7005B5BE3005B5BE3005B5BE3005B5BE3005F5FE7005D5D
      E4003E3EBF00B3B3E40000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000D4ED
      D1005FCE50005ACE4900AADCA400F7FBF700000000000000000000000000B4B4
      B40076767600C1EFEF00A9F6FF00A9F6FF00A8F6FF00A7F6FF00BAEDEF007676
      7600B4B4B400000000000000000000000000000000000000000000000000F2CC
      C000E58D7000EFB7A400FAE4DD00FEF9F700FEF9F700FAE4DD00EFB7A400E58D
      7000F2CCC000000000000000000000000000000000000000000000000000B3B3
      E5004646C2004C4CD5006262EA006B6BF3006A6AF3006161E9004C4CD4004646
      C200B3B3E5000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C5E8C100B4E1AF0000000000000000000000000000000000000000000000
      0000B4B4B400767676007676760076767600767676007676760076767600B4B4
      B400000000000000000000000000000000000000000000000000000000000000
      0000FCEFEB00F4BEAC00EF9B7F00EC8A6A00EC8A6A00EF9B7F00F4BEAC00FCEF
      EB00000000000000000000000000000000000000000000000000000000000000
      0000E5E5F6009393DB005858C7003939BD003939BD005858C7009393DB00E5E5
      F600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000200000000100010000000000000100000000000000000000
      000000000000000000000000FFFFFF008001C003FFFF00008001C003FFFF0000
      0000FC3FFFFF00000000FC3FFFFF00000000F81FE3FF00000000F81FE1FF0000
      0000F00FE0FF00000000F00FF07F00000000E007F83F00000000E007FC1F0000
      0000E007FE0F00000000E007FF0F00000000E007FF8F00000000E007FFFF0000
      0000F00FFFFF00000000F81FFFFF0000E00F8001C0038001C0078001C0038001
      C00FE007E007E007E07FC003C003C003C03F800180018001801F800180018001
      000F800180018001000F8001800180018603800180018001CE03800180018401
      FF01800180018E71FF80800180018421FFC0C003C003C003FFE0E007E007E007
      FFF3F00FF00FF00FFFFFFFFFFFFFFFFF00000000000000000000000000000000
      000000000000}
  end
  object ActionListMain: TActionList
    Images = ImageListMain
    Left = 32
    Top = 520
    object ActionSaveConfigSave: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1086#1089#1085#1086#1074#1085#1091#1102' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1102
      Visible = False
      OnExecute = ActionSaveConfigSaveExecute
    end
    object ActionFileOpen: TAction
      Category = 'File'
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1073#1072#1079#1091
      Hint = #1054#1090#1082#1088#1099#1090#1100' '#1092#1072#1081#1083' '#1073#1072#1079#1099' 1CD'
      ImageIndex = 0
      ShortCut = 16463
      OnExecute = ActionFileOpenExecute
    end
    object ActionBlobViewMode: TAction
      Category = 'BlobView'
      Caption = #1056#1077#1078#1080#1084
      GroupIndex = 1
      OnExecute = ActionBlobViewExecute
    end
    object ActionExit: TAction
      Category = 'File'
      Caption = #1042#1099#1093#1086#1076
      Hint = #1047#1072#1082#1088#1099#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
      ImageIndex = 1
      ShortCut = 121
      OnExecute = ActionExitExecute
    end
    object ActionDebugMessages: TAction
      Category = 'Parameters'
      Caption = #1054#1090#1083#1072#1076#1086#1095#1085#1099#1077' '#1089#1086#1086#1073#1097#1077#1085#1080#1103
      Visible = False
      OnExecute = ActionDebugMessagesExecute
    end
    object ActionBlobViewModeDetect: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = #1040#1074#1090#1086#1086#1087#1088#1077#1076#1077#1083#1077#1085#1080#1077
      Checked = True
      OnExecute = ActionBlobViewModeDetectExecute
    end
    object ActionBlobView: TAction
      Category = 'BlobView'
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1087#1088#1086#1089#1084#1086#1090#1088#1072' Blob '#1087#1086#1083#1077#1081
      GroupIndex = 1
      OnExecute = ActionBlobViewExecute
    end
    object ActionBlobViewANSI: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = 'ANSI'
      GroupIndex = 1
      OnExecute = ActionBlobViewANSIExecute
    end
    object ActionBlobViewUTF8: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = 'UTF-8'
      GroupIndex = 1
      OnExecute = ActionBlobViewUTF8Execute
    end
    object ActionBlobViewUTF16: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = 'UTF-16'
      GroupIndex = 1
      OnExecute = ActionBlobViewUTF16Execute
    end
    object ActionBlobViewMedia: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = 'Media'
      GroupIndex = 1
      OnExecute = ActionBlobViewMediaExecute
    end
    object ActionBlobViewWEB: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = 'WEB'
      GroupIndex = 1
      OnExecute = ActionBlobViewWEBExecute
    end
    object ActionBlobViewHEX: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = 'HEX'
      GroupIndex = 1
      OnExecute = ActionBlobViewHEXExecute
    end
    object ActionBlobViewTextWrap: TAction
      Category = 'BlobView'
      AutoCheck = True
      Caption = #1055#1077#1088#1077#1085#1086#1089#1080#1090#1100' '#1089#1090#1088#1086#1082#1080
      OnExecute = ActionBlobViewTextWrapExecute
    end
    object ActionBlobParseModeDetect: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1040#1074#1090#1086#1086#1087#1088#1077#1076#1077#1083#1077#1085#1080#1077
      Checked = True
      OnExecute = ActionBlobParseModeDetectExecute
    end
    object ActionBlobParseUnzip: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1056#1072#1089#1087#1072#1082#1086#1074#1072#1090#1100' (Inflate)'
      Enabled = False
      OnExecute = ActionBlobParseUnzipExecute
    end
    object ActionBlobParseUnzip2: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1056#1072#1089#1087#1072#1082#1086#1074#1072#1090#1100' 2 (Inflate 2)'
      Enabled = False
      OnExecute = ActionBlobParseUnzip2Execute
    end
    object ActionBlobParseContainer: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1050#1072#1082' '#1082#1086#1085#1090#1077#1081#1085#1077#1088
      Enabled = False
      OnExecute = ActionBlobParseContainerExecute
    end
    object ActionBlobParseValueStorage: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1061#1088#1072#1085#1080#1083#1080#1097#1077' '#1079#1085#1072#1095#1077#1085#1080#1103
      Enabled = False
      OnExecute = ActionBlobParseValueStorageExecute
    end
    object ActionBlobParseValue1C: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1047#1085#1072#1095#1077#1085#1080#1077' 1'#1057
      Enabled = False
      OnExecute = ActionBlobParseValue1CExecute
    end
    object ActionSaveConfig: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1102' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093
      Visible = False
      OnExecute = ActionSaveConfigExecute
    end
    object ActionFindSupplierConfigs: TAction
      Category = 'Utils'
      Caption = #1053#1072#1081#1090#1080' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' '#1087#1086#1089#1090#1072#1074#1097#1080#1082#1072
      Visible = False
      OnExecute = ActionFindSupplierConfigsExecute
    end
    object ActionSaveSupplierConfig: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1102' '#1087#1086#1089#1090#1072#1074#1097#1080#1082#1072
      Visible = False
      OnExecute = ActionSaveSupplierConfigExecute
    end
    object ActionSaveDataFile: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1092#1072#1081#1083' Records '#1090#1072#1073#1083#1080#1094#1099
      Visible = False
      OnExecute = ActionSaveDataFileExecute
    end
    object ActionSaveBlobFile: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1092#1072#1081#1083' Blob '#1090#1072#1073#1083#1080#1094#1099
      Visible = False
      OnExecute = ActionSaveBlobFileExecute
    end
    object ActionSaveIndexFile: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1092#1072#1081#1083' Index '#1090#1072#1073#1083#1080#1094#1099
      Visible = False
      OnExecute = ActionSaveIndexFileExecute
    end
    object ActionDumpIndex: TAction
      Category = 'Test'
      Caption = #1044#1072#1084#1087' '#1080#1085#1076#1077#1082#1089#1072
      OnExecute = ActionDumpIndexExecute
    end
    object ActionExportTableToXML: TAction
      Category = 'Utils'
      Caption = #1069#1082#1089#1087#1086#1088#1090' '#1090#1072#1073#1083#1080#1094#1099' '#1074' XML'
      Hint = #1069#1082#1089#1087#1086#1088#1090' '#1090#1072#1073#1083#1080#1094#1099' '#1074' XML'
      ImageIndex = 20
      Visible = False
      OnExecute = ActionExportTableToXMLExecute
    end
    object ActionAbout: TAction
      Category = 'Help'
      Caption = #1054' '#1087#1088#1086#1075#1088#1072#1084#1084#1077
      OnExecute = ActionAboutExecute
    end
    object ActionViewFileOffset: TAction
      Category = 'Parameters'
      AutoCheck = True
      Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1089#1084#1077#1097#1077#1085#1080#1103' '#1074' '#1092#1072#1081#1083#1077' '#1079#1072#1087#1080#1089#1077#1081' '#1090#1072#1073#1083#1080#1094#1099
      OnExecute = ActionViewFileOffsetExecute
    end
    object ActionShowGUIDs: TAction
      Category = 'Parameters'
      AutoCheck = True
      Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' Bynary(16) '#1082#1072#1082' GUID'
      Checked = True
      ShortCut = 16455
      OnExecute = ActionShowGUIDsExecute
    end
    object ActionOpenBaseNotMonopoly: TAction
      Category = 'Parameters'
      AutoCheck = True
      Caption = #1054#1090#1082#1088#1099#1074#1072#1090#1100' '#1073#1072#1079#1091' '#1085#1077' '#1084#1086#1085#1086#1087#1086#1083#1100#1085#1086' ('#1085#1077#1073#1077#1079#1086#1087#1072#1089#1085#1086')'
      OnExecute = ActionOpenBaseNotMonopolyExecute
    end
    object ActionTestIndexes: TAction
      Category = 'Test'
      Caption = #1055#1088#1086#1074#1077#1088#1082#1072' '#1074#1089#1077#1093' '#1080#1085#1076#1077#1082#1089#1086#1074
      OnExecute = ActionTestIndexesExecute
    end
    object ActionExportTable: TAction
      Category = 'Table'
      Caption = #1069#1082#1089#1087#1086#1088#1090' '#1090#1077#1082#1091#1097#1077#1081' '#1090#1072#1073#1083#1080#1094#1099
      OnExecute = ActionExportTableExecute
    end
    object ActionImportTable: TAction
      Category = 'Table'
      Caption = #1048#1084#1087#1086#1088#1090' '#1090#1077#1082#1091#1097#1077#1081' '#1090#1072#1073#1083#1080#1094#1099
      OnExecute = ActionImportTableExecute
    end
    object ActionExportDataTables: TAction
      Category = 'Table'
      Caption = #1069#1082#1089#1087#1086#1088#1090' '#1090#1072#1073#1083#1080#1094' '#1076#1072#1085#1085#1099#1093
      OnExecute = ActionExportDataTablesExecute
    end
    object ActionImportDataTables: TAction
      Category = 'Table'
      Caption = #1048#1084#1087#1086#1088#1090' '#1090#1072#1073#1083#1080#1094' '#1076#1072#1085#1085#1099#1093
      OnExecute = ActionImportDataTablesExecute
    end
    object ActionEditTable: TAction
      Category = 'Table'
      Caption = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1090#1100
      Hint = #1056#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1090#1100' '#1090#1072#1073#1083#1080#1094#1091
      ImageIndex = 17
      ShortCut = 16453
      Visible = False
      OnExecute = ActionEditTableExecute
    end
    object ActionSaveTableChanges: TAction
      Category = 'Table'
      Caption = #1047#1072#1087#1080#1089#1072#1090#1100
      Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1080#1079#1084#1077#1085#1077#1085#1080#1103
      ImageIndex = 26
      ShortCut = 16467
      Visible = False
      OnExecute = ActionSaveTableChangesExecute
    end
    object ActionDiscardTableChanges: TAction
      Category = 'Table'
      Caption = #1054#1090#1084#1077#1085#1080#1090#1100
      Hint = #1054#1090#1084#1077#1085#1080#1090#1100' '#1080#1079#1084#1077#1085#1077#1085#1080#1103' (Ctrl+Break)'
      ImageIndex = 19
      Visible = False
      OnExecute = ActionDiscardTableChangesExecute
    end
    object ActionEditTableInsert: TAction
      Category = 'Table'
      Caption = #1044#1086#1073#1072#1074#1080#1090#1100' '#1079#1072#1087#1080#1089#1100
      Hint = #1044#1086#1073#1072#1074#1080#1090#1100' '#1079#1072#1087#1080#1089#1100' (Ins)'
      ImageIndex = 21
      Visible = False
      OnExecute = ActionEditTableInsertExecute
    end
    object ActionEditTableDelete: TAction
      Category = 'Table'
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1079#1072#1087#1080#1089#1100
      Hint = #1059#1076#1072#1083#1080#1090#1100' '#1079#1072#1087#1080#1089#1100' (Del)'
      ImageIndex = 22
      Visible = False
      OnExecute = ActionEditTableDeleteExecute
    end
    object ActionStringEditDropDown: TAction
      Category = 'Edit'
      AutoCheck = True
      Caption = '...'
    end
    object ActionNULL: TAction
      Category = 'Edit'
      Caption = 'ActionNULL'
      Hint = #1042#1074#1077#1089#1090#1080' '#1079#1085#1072#1095#1077#1085#1080#1077' NULL'
      ShortCut = 16462
      OnExecute = ActionNULLExecute
    end
    object ActionRestore: TAction
      Category = 'Edit'
      Caption = 'ActionRestore'
      Hint = #1042#1086#1089#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077
      ImageIndex = 24
      ShortCut = 16466
      OnExecute = ActionRestoreExecute
    end
    object ActionClear: TAction
      Category = 'Edit'
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077
      Hint = #1054#1095#1080#1089#1090#1080#1090#1100' '#1079#1085#1072#1095#1077#1085#1080#1077
      ImageIndex = 23
      ShortCut = 16472
      OnExecute = ActionClearExecute
    end
    object ActionFindLostObjects: TAction
      Category = 'Test'
      Caption = #1053#1072#1081#1090#1080' '#1087#1086#1090#1077#1088#1103#1085#1085#1099#1077' '#1086#1073#1098#1077#1082#1090#1099
      OnExecute = ActionFindLostObjectsExecute
    end
    object ActionTestStreamFormat: TAction
      Category = 'Test'
      Caption = #1058#1077#1089#1090' '#1092#1086#1088#1084#1072#1090#1072' '#1087#1086#1090#1086#1082#1072
      OnExecute = ActionTestStreamFormatExecute
    end
    object ActionCreateEmptyObject: TAction
      Category = 'Test'
      Caption = #1057#1086#1079#1076#1072#1090#1100' '#1087#1091#1089#1090#1086#1081' '#1086#1073#1098#1077#1082#1090' '#1074' '#1073#1072#1079#1077
      OnExecute = ActionCreateEmptyObjectExecute
    end
    object ActionCreateObjectFromFile: TAction
      Category = 'Test'
      Caption = #1057#1086#1079#1076#1072#1090#1100' '#1086#1073#1098#1077#1082#1090' '#1080#1079' '#1092#1072#1081#1083#1072
      OnExecute = ActionCreateObjectFromFileExecute
    end
    object ActionCreateAndImportTable: TAction
      Category = 'Table'
      Caption = #1057#1086#1079#1076#1072#1085#1080#1077' '#1080' '#1080#1084#1087#1086#1088#1090' '#1090#1072#1073#1083#1080#1094#1099
      OnExecute = ActionCreateAndImportTableExecute
    end
    object ActionDeleteTable: TAction
      Category = 'Table'
      Caption = #1059#1076#1072#1083#1077#1085#1080#1077' '#1090#1077#1082#1091#1097#1077#1081' '#1090#1072#1073#1083#1080#1094#1099
      OnExecute = ActionDeleteTableExecute
    end
    object ActionEnableEdit: TAction
      Category = 'Edit'
      Caption = #1059#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1088#1077#1078#1080#1084' '#1088#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1103
      OnExecute = ActionEnableEditExecute
    end
    object ActionTestListTables: TAction
      Category = 'Test'
      Caption = #1055#1088#1086#1074#1077#1088#1082#1072' '#1089#1086#1089#1090#1072#1074#1072' '#1090#1072#1073#1083#1080#1094
      OnExecute = ActionTestListTablesExecute
    end
    object ActionCreateTables: TAction
      Category = 'Table'
      Caption = #1048#1084#1087#1086#1088#1090' '#1080' '#1089#1086#1079#1076#1072#1085#1080#1077' '#1090#1072#1073#1083#1080#1094
      OnExecute = ActionCreateTablesExecute
    end
    object ActionReplaceTREF: TAction
      Category = 'Table'
      Caption = #1047#1072#1084#1077#1085#1072' TREF'
      OnExecute = ActionReplaceTREFExecute
    end
    object ActionBlobParseUsersUsr: TAction
      Category = 'BlobParse'
      AutoCheck = True
      Caption = #1044#1077#1096#1080#1092#1088#1086#1074#1072#1090#1100
      Enabled = False
      OnExecute = ActionBlobParseUsersUsrExecute
    end
    object ActionAssociateFile: TAction
      Category = 'Parameters'
      Caption = #1040#1089#1089#1086#1094#1080#1080#1088#1086#1074#1072#1090#1100' '#1092#1072#1081#1083#1099' 1CD '#1089' Tool_1CD'
      OnExecute = ActionAssociateFileExecute
    end
    object ActionXMLSaveBLOBToFile: TAction
      Category = 'Parameters'
      AutoCheck = True
      Caption = #1055#1088#1080' '#1101#1082#1089#1087#1086#1088#1090#1077' '#1074' XML '#1089#1086#1093#1088#1072#1085#1103#1090#1100' BLOB '#1074' '#1086#1090#1076#1077#1083#1100#1085#1099#1077' '#1092#1072#1081#1083#1099
      OnExecute = ActionXMLSaveBLOBToFileExecute
    end
    object ActionXMLUnpackBLOB: TAction
      Category = 'Parameters'
      AutoCheck = True
      Caption = #1055#1088#1080' '#1101#1082#1089#1087#1086#1088#1090#1077' '#1074' XML '#1088#1072#1089#1087#1072#1082#1086#1074#1099#1074#1072#1090#1100' BLOB'
      Checked = True
      Visible = False
      OnExecute = ActionXMLUnpackBLOBExecute
    end
    object ActionFindAndCreateLostTables: TAction
      Category = 'Utils'
      Caption = #1055#1086#1080#1089#1082' '#1080' '#1074#1086#1089#1089#1090#1072#1085#1086#1074#1083#1077#1085#1080#1077' '#1087#1086#1090#1077#1088#1103#1085#1085#1099#1093' '#1090#1072#1073#1083#1080#1094
      OnExecute = ActionFindAndCreateLostTablesExecute
    end
    object ActionDeleteWrongTables: TAction
      Category = 'Utils'
      Caption = #1059#1076#1072#1083#1080#1090#1100' '#1083#1080#1096#1085#1080#1077' '#1090#1072#1073#1083#1080#1094#1099
      Hint = #1059#1076#1072#1083#1080#1090#1100' '#1083#1080#1096#1085#1080#1077' '#1090#1072#1073#1083#1080#1094#1099' (DBCHANGES, *NG, *OG)'
      OnExecute = ActionDeleteWrongTablesExecute
    end
    object ActionFindTableDoubles: TAction
      Category = 'Utils'
      Caption = #1053#1072#1081#1090#1080' '#1076#1091#1073#1083#1080' '#1090#1072#1073#1083#1080#1094
      OnExecute = ActionFindTableDoublesExecute
    end
    object ActionFindAndSaveLostObjects: TAction
      Category = 'Utils'
      Caption = #1053#1072#1081#1090#1080' '#1080' '#1089#1086#1093#1088#1072#1085#1080#1090#1100' '#1087#1086#1090#1077#1088#1103#1085#1085#1099#1077' '#1086#1073#1098#1077#1082#1090#1099
      OnExecute = ActionFindAndSaveLostObjectsExecute
    end
    object ActionCommandLineKeys: TAction
      Category = 'Help'
      Caption = #1050#1083#1102#1095#1080' '#1082#1086#1084#1072#1085#1076#1085#1086#1081' '#1089#1090#1088#1086#1082#1080
      OnExecute = ActionCommandLineKeysExecute
    end
    object ActionTestStreamFormatExternal: TAction
      Category = 'Test'
      Caption = #1058#1077#1089#1090' '#1092#1086#1088#1084#1072#1090#1072' '#1087#1086#1090#1086#1082#1072' '#1074#1085#1077#1096#1085#1077#1075#1086' '#1092#1072#1081#1083#1072
      OnExecute = ActionTestStreamFormatExternalExecute
    end
    object ActionWriteBlobToFile: TAction
      Category = 'Edit'
      Caption = 'ActionWriteBlobToFile'
      Hint = #1047#1072#1087#1080#1089#1072#1090#1100' BLOB/MEMO '#1074' '#1092#1072#1081#1083
      ImageIndex = 18
      OnExecute = ActionWriteBlobToFileExecute
    end
    object ActionReadBlobFromFile: TAction
      Category = 'Edit'
      Caption = 'ActionReadBlobFromFile'
      Hint = #1047#1072#1084#1077#1085#1080#1090#1100' BLOB/MEMO ('#1087#1088#1086#1095#1080#1090#1072#1090#1100' '#1080#1079' '#1092#1072#1081#1083#1072')'
      ImageIndex = 25
      OnExecute = ActionReadBlobFromFileExecute
    end
    object ActionBlobClear: TAction
      Category = 'Edit'
      Caption = 'ActionBlobClear'
      Hint = #1054#1095#1080#1089#1090#1080#1090#1100' BLOB/MEMO'
      ImageIndex = 24
      OnExecute = ActionBlobClearExecute
    end
    object ActionBlobRestore: TAction
      Category = 'Edit'
      Caption = 'ActionBlobRestore'
      Hint = #1042#1086#1089#1089#1090#1072#1085#1086#1074#1080#1090#1100' BLOB/MEMO'
      ImageIndex = 23
      OnExecute = ActionBlobRestoreExecute
    end
    object ActionSaveLastDepotConfig: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1087#1086#1089#1083#1077#1076#1085#1102#1102' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1102' '#1093#1088#1072#1085#1080#1083#1080#1097#1072
      OnExecute = ActionSaveLastDepotConfigExecute
    end
    object ActionSaveDepotConfig: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083' '#1090#1077#1082#1091#1097#1091#1102' '#1074#1077#1088#1089#1080#1102' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
      Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083' '#1090#1077#1082#1091#1097#1091#1102' '#1074#1077#1088#1089#1080#1102' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
      ImageIndex = 27
      Visible = False
      OnExecute = ActionSaveDepotConfigExecute
    end
    object ActionRestoreDATAAllocationTable: TAction
      Category = 'Utils'
      Caption = #1042#1086#1089#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1090#1072#1073#1083#1080#1094#1091' '#1088#1072#1079#1084#1077#1097#1077#1085#1080#1103' '#1092#1072#1081#1083#1072' DATA '#1090#1077#1082#1091#1097#1077#1081' '#1090#1072#1073#1083#1080#1094#1099
      OnExecute = ActionRestoreDATAAllocationTableExecute
    end
    object ActionSaveConfigExtension: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083' '#1090#1077#1082#1091#1097#1077#1077' '#1088#1072#1089#1096#1080#1088#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' ('#1086#1089#1085#1086#1074#1085#1086#1077')'
      Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083' '#1090#1077#1082#1091#1097#1077#1077' '#1088#1072#1089#1096#1080#1088#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' ('#1086#1089#1085#1086#1074#1085#1086#1077')'
      ImageIndex = 27
      Visible = False
      OnExecute = ActionSaveConfigExtensionExecute
    end
    object ActionSaveConfigExtensionDb: TAction
      Category = 'Utils'
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083' '#1090#1077#1082#1091#1097#1077#1077' '#1088#1072#1089#1096#1080#1088#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' ('#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093')'
      Hint = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1092#1072#1081#1083' '#1090#1077#1082#1091#1097#1077#1077' '#1088#1072#1089#1096#1080#1088#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' ('#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093')'
      ImageIndex = 28
      Visible = False
      OnExecute = ActionSaveConfigExtensionDbExecute
    end
    object ActionRecoveryMode: TAction
      Category = 'Parameters'
      AutoCheck = True
      Caption = #1059#1087#1088#1086#1096#1077#1085#1085#1099#1081' '#1088#1077#1078#1080#1084' '#1088#1077#1076#1072#1082#1090#1080#1088#1086#1074#1072#1085#1080#1103
    end
  end
  object ImageListMain: TImageList
    DrawingStyle = dsTransparent
    Left = 32
    Top = 584
    Bitmap = {
      494C01011D001001540210001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000008000000001002000000000000080
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EBEBEB00D5D5D500D3D3D300D3D3
      D300D3D3D300D3D3D300D5D5D500EBEBEB000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F7F7F700E9E9E900D9D9
      D900D3D3D300D3D3D300D3D3D300D3D3D30074B9690066B15A0087CF7D005F9C
      4D0085CE7C0085CD7B0066B25B0088CC7C000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EBEBEB00B3B1AD00A9A49E00AFA9
      A100BBB4AB00BEB7AE00C2B9B100BFB8B00068AC5B0078D17B00DAF5D5006595
      6100D2F5CB00D2F3CE0079D27C0081C776000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C4C2BD00BEB8AE00D0C8BF00D6CD
      C500DDD3CB00E4DAD200EADFD800E5DBD50074BE6C0076CF790090DC8900A4E8
      9800A2E696008FDB880077D07A0081C776000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C0BDB800C9C2B800CDC5BC00D4CB
      C300DBD2CA00E3D8D100E9DED700E4DAD30072BC6A0073CC760083D27E008CD6
      84008CD6840083D27E0074CD77007FC775000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C1BEB900D6CFC600E0DAD200DBD4
      CD00D9D3CA00DAD3CB00DBD5CD00DBD3CD0070BA680070C97300BDEBB900ABDC
      A500ABDCA500BDEBB90071CA74007FC675000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C9C7C300C5BDB400BBB2A800CBC2
      B900D8CFC600E5DBD400ECE0D900E6DDD60071BC6A0070C97300C5F0C400B1E3
      AD00B1E3AD00C5F0C40070C973007FC775000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C7C5C000C5BEB400CFC7BE00D5CC
      C400DCD3CB00E3D9D200E9DED700E4DAD3007EC474006CBF6A0083CD7B0083CD
      7B0085CF7D0087D07E0072C5700093D587000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C4C0BB00C8C0B700D4CDC400E3DC
      D400ECE6DE00F0E9E100F3EBE400F0E9E200EDE7E100E5DFD900D6D0C900C9C2
      BB00C5C3BF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C2BD00DCD5CD00C6BFB600C1B8
      AF00C3BAB000CDC3BB00D3C8C000CFC5BD00C5BBB300C4BCB300C9C2BA00DBD5
      CD00C7C4C0000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000CECCC800BAB2A800CEC6BD00D6CD
      C500DDD3CC00E4DAD200EADFD800E5DBD300DED4CC00D7CEC600D0C8BF00BFB7
      AE00CECCC8000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C6C3BD00C9C1B800CDC5BC00D4CB
      C200DBD1C900E2D8D100E9DED700E3D9D200DCD2CA00D5CCC300CEC6BD00CAC2
      B900C6C3BD000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C7C3BD00D5CEC500EAE4DD00EBE5
      DD00E9E3DB00E8E1D900E8E1D900E8E1D900E9E3DB00EBE5DD00EBE5DD00D4CD
      C500C7C3BD000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000C7C3BD00EAE4DB00D2C9BF00D2C9
      BF00D3CABF00D3CAC000D3CAC000D3CAC000D3CABF00D2C9BF00D2C9BF00EAE4
      DB00C8C5BF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000D5D2CD00D4CBC100E2DAD000E1D8
      CE00E0D8CE00E0D8CE00E0D8CE00E0D8CE00E0D8CE00E1D8CE00E2DAD000D5CC
      C200D5D2CD000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000DDDAD600CEC8C000C7BF
      B500CDC5BA00CDC5BA00CDC5BA00CDC5BA00CDC5BA00C7BFB500CEC8C000DDDA
      D600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E6DED500BFA99300AE947A00E1E1E100E5E5E500E5E5E500E5E5
      E500E5E5E500E5E5E500E5E5E500EFEFEF0000000000FDFDFD00F3F3F300E9E9
      E900E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E9E9E900F3F3F300FDFDFD00000000000000000000000000000000000000
      000000000000000000000000000000000000EBEBEB14D5D5D52AD3D3D32CD3D3
      D32CD3D3D32CD3D3D32CD5D5D52AEBEBEB140000000000000000D8A9AB00D18A
      8E00C2A5A600000000000000000000000000000000000000000000000000EBB3
      B500D28F9200BC999A00F6F6F60000000000000000000000000000000000D6C9
      B900B1987A00AA8C6900BC9B760074532B00818171008B8B7A008F8F7E008F8F
      7E008F8F7E008F8F7E008F8F7E00A3A3990000000000FBFBFB00E9E9E900D5D5
      D500BDC3BD007FA07E00559054003B8139003B813900559054007FA07E00BDC3
      BD00D3D3D300E7E7E700FBFBFB000000000000000000F7F7F708E9E9E916D9D9
      D926D3D3D32CD3D3D32CD3D3D32CD3D3D32CBFA67295D5A44AFFD7BC85FFA68B
      53FFD7BB82FFD5BA83FFD5A44BFFD1B8878300000000FFA7AA00FF7B8100FF9F
      A400FF7D8300BF989A0000000000000000000000000000000000FFB1B500FF8F
      9300FF9DA200FF717800BC989A0000000000EFEFEF00DBDBDB00A18E7800A987
      6100C5A48000D4B39100D7B694009D7C5A00E5E5E500E5E5E400FEFEFD00FEFE
      FC00FDFDFA00FCFCF900FEFEF90096968400000000000000000000000000B4CB
      B300459641002CA7230025C6170022D4120022D4120025C617002CA723004596
      4100B4CBB300000000000000000000000000EBEBEB14B3B1AD85A9A49EC5AFA9
      A1F1BBB4ABFFBEB7AEFFC2B9B1FFBFB8B0FFB89A60FFEEBF71FFF0E9E3FF8881
      7BFFF0E6DBFFECE5E0FFEEC072FFCFB47E8B00000000FF4B5300FE212A00FC3D
      4500F87B8000FF818800BF9698000000000000000000FFA8AC00FF8B8F00F567
      6C00FB303900FF1F2900D05F640000000000F7F7F700EDEDED00A5886400D8B8
      9600CFAF8D00CFAF8D00D8B896009F7F5C00E4E4E400E3E3E200FCFCFB00FBFB
      F800FAFAF600F8F8F400FBFBF600A9A997000000000000000000B6D8B3003D9F
      350027C5180023D1120022B6110022D1110022D1110022D1110022D1110025C3
      16003C9F3400B6D8B3000000000000000000C4C2BD7ABEB8AEFFD0C8BFFFD6CD
      C5FFDDD3CBFFE4DAD2FFEADFD8FFE5DBD5FFCAAB70FFECBD6FFFEBC88DFFF1D4
      A1FFEFD2A0FFEAC78DFFECBE71FFCFB47E8B00000000FF989D00FF242E00F948
      4F00F56B7100FF9A9F00FE828700C3969800E9A4A700FF909500FF8B8F00F65F
      6400FB3C4400FF222D00EDB0B300000000000000000000000000AE947100D9BB
      9900D0B29000D0B29000D9BB9900A0815E00B7B7A800B6B6A700DBDBCA00D1D1
      C000F8F8F400F7F7F100FAFAF400B1B1A00000000000E6F2E5004BA742002DC1
      1E0023C8120022B21100E6E6E60022B2110022C8110022C8110022C8110022C8
      110026BE160049A64100E6F2E50000000000C0BDB88BC9C2B8FFCDC5BCFFD4CB
      C3FFDBD2CAFFE3D8D1FFE9DED7FFE4DAD3FFC9A96FFFEBBB6BFFE6BE7FFFE6C2
      8BFFE6C28BFFE6BE7FFFEBBC6DFFCFB47D8B0000000000000000FF8E9300FC3D
      4500F5636900FF828700FFB1B500FF909600FF9A9E00FFA4A800FE767C00F657
      5E00FD343D00FFB2B60000000000000000000000000000000000B1997500DBBE
      9C00D2B59300D2B59300DBBE9C00A2846000D5D5C600D4D4C500EBEBDA00E9E9
      D800F7F7F100F5F5EF00FAFAF200B5B5A4000000000098CD93003AB12C0026C0
      150022AD1100DEDEDE00E2E2E200E6E6E60022AD110022BE110022BE110022BE
      110022BE110030AB240098CD930000000000C1BEB98BD6CFC6FFE0DAD2FFDBD4
      CDFFD9D3CAFFDAD3CBFFDBD5CDFFDBD3CDFFC6A86CFFEAB866FFDED8D3FFD1C8
      BEFFD1C8BEFFDED8D3FFEAB968FFCFB37D8B0000000000000000FFFBFB00FF8A
      8F00F64D5500FC6F7500FE848900FF949900FF8F9400FE797E00F8636900FF44
      4D00FFA5A9000000000000000000000000000000000000000000B39C7900DCC0
      9E00D3B79500D3B79500DCC09E00A4876300B5B5A600C4C4B400C7C7B600D6D6
      C500D4D4C300CACAB900F9F9EF00B8B8A7000000000061B4580042BF330023AE
      1200D5D5D500DADADA00DEDEDE00E2E2E200E6E6E60022A8110022B4110022B4
      110022B4110029B21A0061B4580000000000C9C7C374C5BDB4FFBBB2A8FFCBC2
      B9FFD8CFC6FFE5DBD4FFECE0D9FFE6DDD6FFCAA96EFFE9B867FFE1DFDAFFD4CF
      C5FFD4CFC5FFE1DFDAFFE9B868FFCFB37C8B0000000000000000000000000000
      0000FD828800FA596000FE737800FE747900FC646A00F75B6100F7414800EAA1
      A500000000000000000000000000000000000000000000000000B59F7B00DDC3
      A100D4BA9800D4BA9800DDC3A100A6896500D3D3C400D1D1C200E7E7D600E5E5
      D400E4E4D300E2E2D100F6F6E800BCBCAB000000000045AA390051C84000A9D7
      A200D5D5D500EBEBEB0022A51100DEDEDE00E2E2E200E6E6E60022A3110022AA
      110022AA110029AE180045AA390000000000C7C5C078C5BEB4FFCFC7BEFFD5CC
      C4FFDCD3CBFFE3D9D2FFE9DED7FFE4DAD3FFCDB07DFFD8AF62FFD9BB7FFFD9BB
      7FFFDABC81FFDABD84FFD9B46CFFD9C293760000000000000000000000000000
      0000FF8C9100F7515800FC676C00FE6B7100FA5E6400FB384000FF202B00C18E
      9100000000000000000000000000000000000000000000000000B8A27F00DFC7
      A500D6BE9C00D6BE9C00DFC7A500A78C6600B3B3A300C0C0B100C3C3B200C2C2
      B100C0C0AF00BEBEAD00F4F4E300BFBFAE000000000046AD390054CB43003CB3
      2B00F8F8F8002DA81C0023A21200229F1100DEDEDE00E2E2E200E6E6E600229E
      110022A111002DAA1C0046AD390000000000C4C0BB8BC8C0B7FFD4CDC4FFE3DC
      D4FFECE6DEFFF0E9E1FFF3EBE4FFF0E9E2FFEDE7E1FFE5DFD9FFD6D0C9FFC9C2
      BBFFC5C3BF8B000000000000000000000000000000000000000000000000FE9C
      A100F1464D00F15C6200FA5F6500FC666C00FC686E00FC333C00FD030C00FB0C
      1700BC9294000000000000000000000000000000000000000000B9A58100E0CA
      A800D7C19F00D7C19F00E0CAA800A98E6900CFCFC000CECEBE00E4E4D300F0F0
      E600EBEBDD00E7E7D600F2F2E100C2C2B1000000000063BC580057CD470047BE
      36003EB52D0047BE360041B9300037AF26002DA41C00E2E2E200E3E3E300E7E7
      E700269E15003BB32B0063BC580000000000C6C2BD89DCD5CDFFC6BFB6FFC1B8
      AFFFC3BAB0FFCDC3BBFFD3C8C0FFCFC5BDFFC5BBB3FFC4BCB3FFC9C2BAFFDBD5
      CDFFC7C4C0890000000000000000000000000000000000000000FD939700F95A
      6100F66C7200F9676D00FC747900FD5E6600FD697000FB686E00F9050F00FD00
      0800FF010D00B991930000000000000000000000000000000000BCA88500E2CC
      AA00D9C3A100D9C3A100E2CCAA00AB916B00AFAFA000BDBDAE00C8C8B700EBEB
      DD00A4A49300A4A49300A4A49300A6A69400000000009AD5930052C6430054CB
      43004EC53D004EC53D004EC53D004EC53D004EC53D0044BB330000000000A7E2
      9E0052C941004CC03C009AD5930000000000CECCC864BAB2A8FFCEC6BDFFD6CD
      C5FFDDD3CCFFE4DAD2FFEADFD8FFE5DBD3FFDED4CCFFD7CEC6FFD0C8BFFFBFB7
      AEFFCECCC86400000000000000000000000000000000FDA5A800FD626900FD88
      8D00FE808500FF8F9400FE777E00FD8F9100FD808400FF8D9200FB4F5600FA00
      0200FD000B00F7091300C0A2A300000000000000000000000000BEAA8700E3CF
      AD00DAC6A400DAC6A400E3CFAD00B0997200DBDBD400D8D8CF00EBEBDD00E7E7
      D600B6B6A50000000000C8C8B700ECECE60000000000E6F5E50053BD450061D8
      500058CF470057CE460057CE460057CE460057CE460057CE46004AC1390052C9
      41005CD34A0052BC4400E6F5E50000000000C6C3BD8BC9C1B8FFCDC5BCFFD4CB
      C2FFDBD1C9FFE2D8D1FFE9DED7FFE3D9D2FFDCD2CAFFD5CCC3FFCEC6BDFFCAC2
      B9FFC6C3BD8B00000000000000000000000000000000FB646B00FFACB000FF98
      9C00FFAFB300FEA5AA00FEA7A9000000000000000000FD8B8F00FFB2B600FA13
      1D00FA000000FF071300D2535900000000000000000000000000BFAC8900E4D2
      B000DBC9A700DBC9A700E5D2B000B5A27B00EDEDE300F4F4E700F3F3E300F2F2
      E100C2C2B100A3987200ECECE600000000000000000000000000B8E4B3004CBE
      3D0064DB530063DA52005FD64E005FD64E005FD64E005FD64E0062D9510062DA
      50004BBD3C00B8E4B3000000000000000000C7C3BD8BD5CEC5FFEAE4DDFFEBE5
      DDFFE9E3DBFFE8E1D9FFE8E1D9FFE8E1D9FFE9E3DBFFEBE5DDFFEBE5DDFFD4CD
      C5FFC7C3BD8B00000000000000000000000000000000FC8C8F00FFC9CC00FFE2
      E400FCA7AB00FCA0A100000000000000000000000000FFF9F900FD929400FD7F
      8500FA727200FF6E7400D0A3A400000000000000000000000000C0AE8B00E6D4
      B200E2D1AF00DAC69F00C5AC7E00967F4900B7AC8700B7AC8700B7AC8700B7AC
      8700BCB18C00B6A581000000000000000000000000000000000000000000B8E5
      B30054C1450056CE450065DD54006CE35B006CE35A0065DC540055CD440054C1
      4500B8E5B300000000000000000000000000C7C3BD8BEAE4DBFFD2C9BFFFD2C9
      BFFFD3CABFFFD3CAC0FFD3CAC0FFD3CAC0FFD3CABFFFD2C9BFFFD2C9BFFFEAE4
      DBFFC8C5BF8B00000000000000000000000000000000FFFAFA00FD8E9300FD8C
      9200FDAEB100000000000000000000000000000000000000000000000000FD99
      9E00FC8B9100FFA1A60000000000000000000000000000000000C2B08D00D7C1
      9600BFA57200BEA47200D2BD9200E2D0AD00E6D4B200E6D4B200E6D4B200E6D4
      B200EAD9B700C2B08D0000000000000000000000000000000000000000000000
      0000E7F6E5009BDB930065C758004ABD39004ABD390065C758009BDB9300E7F6
      E50000000000000000000000000000000000D5D2CD70D4CBC1FFE2DAD0FFE1D8
      CEFFE0D8CEFFE0D8CEFFE0D8CEFFE0D8CEFFE0D8CEFFE1D8CEFFE2DAD0FFD5CC
      C2FFD5D2CD700000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D2C5AB00C3B2
      8F00C3B28F00C3B28F00C3B28F00C3B28F00C3B28F00C3B28F00C3B28F00C3B2
      8F00C3B28F00D2C5AB0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000DDDAD662CEC8C0AFC7BF
      B5EDCDC5BAFFCDC5BAFFCDC5BAFFCDC5BAFFCDC5BAFFC7BFB5EDCEC8C0AFDDDA
      D66200000000000000000000000000000000D8E9EC00D8E9EC00D8E9EC00B584
      8400B5848400B5848400B5848400B5848400B5848400B5848400B5848400B584
      8400B5848400B5848400B5848400B584840000000000D3D3D300A7927D00AF8D
      6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B004C78
      2B00256E14004A784000D3D3D3000000000000000000D3D3D300A7927D00AF8D
      6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D6B00AF8D
      6B00AF8D6B00A7927D00D3D3D3000000000000000000FDFDFD00F3F3F300E9E9
      E900E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E9E9E900F3F3F300FDFDFD0000000000D8E9EC00D8E9EC00D8E9EC00C6A5
      9C00FFEFD600F7E7C600F7DEBD00F7DEB500F7D6AD00F7D6A500EFCE9C00EFCE
      9400EFCE9400EFCE9400F7D69C00B5848400F7F7F700E9E9E900BA987700FDF4
      E100FBF2DD00FBF2DC00FAF1DC00FAF1DB00FAF0DB00F9F0DA00F9EFDA003B98
      2C002BDF1A002D861700E9E9E900F7F7F700F7F7F700E9E9E900BA987700FDF4
      E100FBF2DD00FBF2DC00FAF1DC00FAF1DB00FAF0DB00F9F0DA00F9EFDA00F9EE
      D900FCF2DF00BA987700E9E9E900F7F7F70000000000FBFBFB00E9E9E900D5D5
      D500CCC8C500C4B19D00BF976F00B0763D00AC6F3300B6855500B59A7F00C7C2
      BD00D3D3D300E7E7E700FBFBFB0000000000D8E9EC00D8E9EC00D8E9EC00C6A5
      9C00FFEFD600F7E7CE00F7DEC600F7DEBD00F7D6B500F7D6A500EFCE9C00EFCE
      9C00EFCE9400EFCE9400EFCE9C00B5848400FFFFFF00FFFFFF00C6A58400FBF2
      DD00F6EED400F5EDD300F5ECD200F4EBD100F4EAD0006AB253003C9F29003B9E
      29003DE22C0031911A003DA231006EBA6600FFFFFF00FFFFFF00C6A58400FBF2
      DD00F6EED400F5EDD300F5ECD200F4EBD100F4EAD000625DA100302E8F00302E
      8F003230920027208000313198006666B500000000000000000000000000ECE5
      DD00D3B08900DAA95F00EDB13900FCB61700FCB20A00E9A31500CE8C2500B477
      3400D6C5B400000000000000000000000000D8E9EC00D8E9EC00D8E9EC00C6AD
      A500FFEFE700F7E7D600F7E7CE00F7DEC600F7DEBD00F7D6B500F7D6AD00EFCE
      9C00EFCE9C00EFCE9400EFCE9C00B5848400FFFFFF00FFFFFF00CDAD8D00FBF2
      DC00F5EDD300F5ECD200F4EBD100F4EAD000F3E8CE003DA6290052E7410052E7
      410052E7410052E7410052E741003EA93100FFFFFF00FFFFFF00CDAD8D00FBF2
      DC00F5EDD300F5ECD200F4EBD100F4EAD000F3E8CE00302EA3005E5EF7005E5E
      F7005E5EF7005E5EF7005E5EF7003131AC000000000000000000F6EFE800E0C4
      A600FAD38000F8C55B00F8BE4500F9BC3C00F9B93100F7B32300F6AE1800F7B2
      2100BB7D3700E3CDB6000000000000000000D8E9EC00D8E9EC00D8E9EC00C6AD
      A500FFF7E700F7E7D600F7E7CE00F7E7C600F7DEC600F7DEB500F7D6B500F7D6
      AD00EFCE9C00EFCE9C00EFCE9400B5848400FFFFFF00FFFFFF00CFB09000FAF1
      DC00F5ECD200F4EBD100F4EAD000F3E8CE00F2E7CD006BBA52003FAC28003EAB
      280066EB550037A01D0040B0310070C36600FFFFFF00FFFFFF00CFB09000FAF1
      DC00F5ECD200F4EBD100F4EAD000F3E8CE00F2E7CD00615CB500302EAE00302D
      AE00322FB1002A23A2003131B8006666C90000000000FCFBF900E9D6C100F9DE
      AA00F6CF8500EFC57800D3A26200BD844800B97C3D00CB914500E9B04500F0AC
      2A00F1B33800BC813E00F6EEE60000000000000000000000000000000000EBE1
      DD0000000000FFF8F50000000000FCF5EE0000000000FCF1E80000000000FCEE
      E100000000000000000000000000E1CCCC00FFFFFF00FFFFFF00D2B39400FAF1
      DB00F4EBD100F4EAD000F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C80040B0
      280075EE640039A61D00FFFFFF00FFFFFF00FFFFFF00FFFFFF00D2B39400FAF1
      DB00F4EBD100F4EAD000F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1
      C700F7EBD500D2B39400FFFFFF00FFFFFF0000000000F7F0E800EEDABF00F5DA
      AB00EFCC9500D4AB8000E7D1BA000000000000000000E2C7AB00BE824200E5AB
      4B00E8A83A00D2984900DDBD9B00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00D4B69700FAF0
      DB00F4EAD000F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C7006CBE
      4F0042B72B006BAB4100FFFFFF00FFFFFF00FFFFFF00FFFFFF00D4B69700FAF0
      DB00F4EAD000F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDF
      C500F7EAD400D4B69700FFFFFF00FFFFFF0000000000F4EADE00F3E1C400F2D8
      B100E3C29B00F0E1D10000000000000000000000000000000000E4CAAD00CE92
      4900E1A44800E2AC5A00CF9E6800000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000E1CCCC00FFFFFF00FFFFFF00D7BA9C00F9F0
      DA00F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDB
      C100F5E7D100D7BA9C00FFFFFF00FFFFFF00FFFFFF00FFFFFF00D7BA9C00F9F0
      DA00F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDB
      C100F5E7D100D7BA9C00FFFFFF00FFFFFF0000000000F6EDE200F9EDDB00F6E7
      D300F2E3D300000000000000000000000000000000000000000000000000CA8E
      4A00DDA25300E9B96B00CA8E4A00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F7DEB500B5848400FFFFFF00FFFFFF00D9BDA000F9EF
      DA00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6
      BC00F4E4CE00D9BDA000FFFFFF00FFFFFF00FFFFFF00FFFFFF00D9BDA000F9EF
      DA00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6
      BC00F4E4CE00D9BDA000FFFFFF00FFFFFF0000000000DCAF7C00D0944F00D094
      4F00D0944F00D0944F00D0944F00DCAF7C00000000000000000000000000D094
      4F00DFA55E00EDC07600D0944F00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000F7D6B500B5848400FFFFFF00FFFFFF00DCBFA300F9EE
      D900F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0
      B600F3E2CC00DCBFA300FFFFFF00FFFFFF00FFFFFF00FFFFFF00DCBFA300F9EE
      D900F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0
      B600F3E2CC00DCBFA300FFFFFF00FFFFFF0000000000D69A5300F9D88E00F2CA
      8200F2CA8200FFE59900D69A5300ECD1B1000000000000000000EDD3B400DA9F
      5900E4AD6900ECC17800DEAD7300000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000C6BDAD00B5848400FFFFFF00FFFFFF00DDC2A600F8EE
      D800F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CD
      B300F2E1CC00DDC2A600FFFFFF00FFFFFF00FFFFFF00FFFFFF00DDC2A600F8EE
      D800F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CD
      B300F2E1CC00DDC2A600FFFFFF00FFFFFF0000000000DCA05800F5CF8600EAB7
      7200FBDB9000DCA05800E5BA86000000000000000000F0D5B600DCA05800E8B5
      6E00EEBE7800E9B97000ECCDA70000000000000000000000000000000000F5E8
      E10000000000000000000000000000000000000000000000000000000000FCF5
      EE0000000000E1D3D000B58C8400B5848400FFFFFF00FFFFFF00DFC4A900F8ED
      D700F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300E5CC
      B200F2E1CC00DFC4A900FFFFFF00FFFFFF00FFFFFF00FFFFFF00DFC4A900F8ED
      D700F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300E5CC
      B200F2E1CC00DFC4A900FFFFFF00FFFFFF0000000000E2A55B00F9D48A00FCDC
      9100F2C27A00F9D48A00E9B66B00E2A55B00E2A55B00E8B16700F0C07800F4C4
      7D00FCDB9100E4A96000FBF3EA0000000000D8E9EC00D8E9EC00D8E9EC00E7C6
      B500000000000000000000000000000000000000000000000000FFFFF700E7CE
      CE00BD8C7300EFB57300EFA54A00C6846B00FFFFFF00FFFFFF00E1C6AC00F8EC
      D700EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300C2A48500C2A4
      8500C2A48500C6A58400FFFFFF00FFFFFF00FFFFFF00FFFFFF00E1C6AC00F8EC
      D700EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300C2A48500C2A4
      8500C2A48500C6A58400FFFFFF00FFFFFF0000000000E6AA5F00FFE59900E6AA
      5F00FBD78D00FAD08600FBD28700FCD58A00FBD38800FACE8400FACF8500FDDE
      9300E9B16500F5DFC3000000000000000000D8E9EC00D8E9EC00D8E9EC00EFCE
      BD0000000000000000000000000000000000000000000000000000000000E7D6
      CE00C6947B00FFC67300CE947300D8E9EC00FFFFFF00FFFFFF00E2C8AD00F7EB
      D600EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300E5CCB200D2B69A00FFF6
      E500E2C8AD00F4EBE200FFFFFF00FFFFFF00FFFFFF00FFFFFF00E2C8AD00F7EB
      D600EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300E5CCB200D2B69A00FFF6
      E500E2C8AD00F4EBE200FFFFFF00FFFFFF0000000000EAAD6300EAAD6300F1CA
      9900EDB67100F3C47700FAD38700FEDC9100FEDC9100FAD38700F3C47700EBB1
      6600F7E0C500000000000000000000000000D8E9EC00D8E9EC00D8E9EC00E7C6
      B500FFF7F700FFF7EF00FFF7EF00FFF7EF00FFF7EF00FFF7EF00FFF7EF00E7CE
      CE00C6947B00CE9C8400D8E9EC00D8E9EC00FFFFFF00FFFFFF00E3CAB000FBF0
      DD00F7E9D400F5E7D100F4E4CE00F3E2CC00F2E1CC00F2E1CC00DCC2A700E3CA
      B000F5ECE300FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00E3CAB000FBF0
      DD00F7E9D400F5E7D100F4E4CE00F3E2CC00F2E1CC00F2E1CC00DCC2A700E3CA
      B000F5ECE300FFFFFF00FFFFFF00FFFFFF0000000000F1C48C00F7DCBA000000
      0000FCF5EB00F5D5AE00F0BF8100EDB16500EDB16500F0BF8100F5D5AE00FCF5
      EB0000000000000000000000000000000000D8E9EC00D8E9EC00D8E9EC00E7C6
      B500EFCEB500EFCEB500EFCEB500EFCEB500E7C6B500E7C6B500EFCEB500D6BD
      B500BD847B00D8E9EC00D8E9EC00D8E9EC00FFFFFF00FFFFFF00EBD8C500E4CB
      B200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200F5EC
      E300FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00EBD8C500E4CB
      B200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200F5EC
      E300FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000EDEDED00D3D3D300A7927D00AF8D
      6B00AF8D6B00AF8D6B00AF8D6B00000000003962680087806A00AF8D6B00AF8D
      6B00AF8D6B00A7927D00D3D3D300EDEDED00D8E9EC00D8E9EC00AD5A5A00AD52
      5200A54A4A00AD949400C6CEC600CECEC600CECEC600C6CEC600C6CEC600B59C
      9C009C4242009C424200A5525200D8E9EC0000000000FDFDFD00F3F3F300E9E9
      E900E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E9E9E900F3F3F300FDFDFD00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F7F7F700E9E9E900BA987700FDF4
      E100FBF2DD00FBF2DC00FAF1DC007EAFB600D9F4FF00409BA600B9D0C700F9EE
      D900FCF2DF00BA987700E9E9E900F7F7F700D8E9EC00BD7B7300CE636300CE6B
      6B00B55A5A009C848400BDA5A500E7CECE00FFF7F700FFFFF700F7F7F700CEB5
      B500942929009C313100C65A5A00AD5A5A0000000000FBFBFB00E9E9E900D5D5
      D500BBBBC1007E7EA000545490003939810039398100545490007E7EA000BBBB
      C100D3D3D300E7E7E700FBFBFB00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C6A58400FBF2
      DD00F6EED400F5EDD300F5ECD200D1DACA0048B0B90079E6F7003F98A300B2C7
      BA00F8EDD800C6A584000000000000000000D8E9EC00BD7B7300CE636300CE63
      6300B55A5A009C7B7B009C424200B5737300E7DEDE00FFF7F70000000000D6B5
      B500943131009C313100BD5A5A00AD5A5A00000000000000000000000000B3B3
      CB00414196002323A7001717C6001212D5001212D5001717C6002323A7004141
      9600B3B3CB000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CDAD8D00FBF2
      DC00F5EDD300F5ECD200F4EBD100F4EAD000B8D7C90048AFB80079E6F7003E98
      A200B8CFC600CDAD8D000000000000000000D8E9EC00BD7B7300CE636300CE63
      6300B55A5A00AD8484009C3939009C393900CEBDBD00EFEFEF0000000000E7C6
      C6009429290094313100BD5A5A00AD5A5A000000000000000000B3B3D8003535
      9F001818C5001212B6001111D1001111D1001111D1001111D1001111B6001616
      C40034349F00B3B3D80000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000CFB09000FAF1
      DC00F5ECD200F4EBD100F4EAD000F3E8CE00F2E7CD00B7D4C70047AEB70079E6
      F700409AA500A29E88000000000000000000D8E9EC00BD7B7300CE636300CE63
      6300B55A5A00B58C8C009C4A4A0094313100A59C9C00D6D6D60000000000E7C6
      C6009429290094313100BD5A5A00AD5A5A0000000000E5E5F2004242A7002020
      C2001212B300DCDCDC001111B2001111C8001111C8001111B200EEEEEE001111
      B2001616BE004141A600E5E5F200000000000000000000000000000000007F7F
      7F007F7F7F007F7F7F007F7F7F00666666006666660066666600666666000000
      0000000000000000000000000000000000000000000000000000D2B39400FAF1
      DB00F4EBD100F4EAD000F3E8CE00F2E7CD00F2E5CB00F1E4CA00B6D2C40047AD
      B60079E6F7007B6C5C00D4D4D40000000000D8E9EC00BD7B7300CE636300CE63
      6300BD5A5A00C6948C00C6949400B5848400AD8C8C00BDA5A500E7C6C600DEAD
      AD00A5393900A5393900C65A5A00AD5A5A00000000009393CD002F2FB3001616
      C100D1D1D100D6D6D600DCDCDC001111AD001111AD00EAEAEA00EEEEEE00EEEE
      EE001111BE002424AC009393CD0000000000000000000000000000000000CCCC
      CC007F7F7F007F7F7F007F7F7F007F7F7F006666660066666600666666000000
      0000000000000000000000000000000000000000000000000000D4B69700FAF0
      DB00F4EAD000F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700B5CF
      C100AEA69A00F7F7F7007E7E9700B9B9E400D8E9EC00BD7B7300CE636300CE63
      6300CE636300CE636300CE636300CE636300CE636300C65A5A00C65A5A00CE63
      6300CE636300CE636300CE6B6B00AD525A00000000005858B4003636C2001212
      B4001111B400D1D1D100D6D6D600DCDCDC00E2E2E200E6E6E600EAEAEA001111
      B4001111B4001B1BB2005858B40000000000000000000000000000000000CCCC
      CC00CCCCCC007F7F7F007F7F7F007F7F7F007F7F7F0066666600666666000000
      0000000000000000000000000000000000000000000000000000D7BA9C00F9F0
      DA00F3E8CE00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDB
      C100DDD0BE00877789008080FF003E3EC300D8E9EC00BD7B7300B5525200B55A
      5A00C6848400D6A5A500D6ADAD00D6ADA500D6ADAD00D6A5A500D6A5A500D6AD
      A500D6ADAD00D69C9C00CE636300AD525200000000003939AA004545CD002626
      B5001414AC001111AA00D1D1D100D6D6D600DCDCDC00E2E2E2001111AA001111
      AA001111AA001919AF003939AA00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D9BDA000F9EF
      DA00F2E7CD00F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6
      BC00F4E4CE00B39AA5005050E800BFBFF000D8E9EC00BD7B7300AD524A00E7CE
      CE00F7F7F700F7F7EF00F7F7F700F7F7F700F7F7F700F7F7F700F7F7F700F7F7
      F700F7F7F700DEBDBD00C65A5A00AD525A00000000003939AD004A4AD1003333
      BB002E2EB80013139F00CECECE00D1D1D100D6D6D600DCDCDC0011119E001111
      A1001111A1001E1EAC003939AD00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DCBFA300F9EE
      D900F2E5CB00F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0
      B600F3E2CC00DCBFA3000000000000000000D8E9EC00BD7B7300B5524A00EFD6
      D600FFF7F700F7EFEF00F7EFEF00F7EFEF00F7EFEF00F7EFEF00F7EFEF00F7EF
      EF00F7F7F700DEBDBD00C65A5A00AD525A00000000005858BC004E4ED4003737
      BF002323AB0000000000F7F7F700E8E8E800DEDEDE00DBDBDB00DCDCDC001111
      9B001616A0002F2FB5005858BC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DDC2A600F8EE
      D800F1E4CA00F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CD
      B300F2E1CC00DDC2A6000000000000000000D8E9EC00BD7B7300B5524A00EFD6
      D600EFEFEF00D6D6D600D6D6D600D6D6D600D6D6D600D6D6D600D6D6D600D6D6
      D600EFEFEF00DEBDBD00C65A5A00AD525A00000000009393D5004848CC004949
      D1000000000000000000000000004242CA004242CA0000000000000000000000
      00004747CF004141C4009393D500000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000DFC4A900F8ED
      D700F0E2C800EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300E5CC
      B200F2E1CC00DFC4A9000000000000000000D8E9EC00BD7B7300B5524A00EFD6
      D600EFEFEF00DED6D600DEDEDE00DEDEDE00DEDEDE00DEDEDE00DEDEDE00DED6
      D600EFEFEF00DEBDBD00C65A5A00AD525A0000000000E5E5F5004545BD005B5B
      E2005151D900000000004F4FD7004F4FD7004F4FD7004F4FD700000000005151
      D9005555DD004545BD00E5E5F500000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000E1C6AC00F8EC
      D700EFE1C700EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300C2A48500C2A4
      8500C2A48500C6A584000000000000000000D8E9EC00BD7B7300B5524A00EFD6
      D600F7F7EF00E7DEDE00E7DEDE00E7DEDE00E7DEDE00E7DEDE00E7DEDE00E7DE
      DE00EFEFEF00DEBDBD00C65A5A00AD525A000000000000000000B3B3E4003E3E
      C0006060E8005F5FE7005B5BE3005B5BE3005B5BE3005B5BE3005F5FE7005D5D
      E4003E3EBF00B3B3E40000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000E2C8AD00F7EB
      D600EFDFC500EDDBC100EAD6BC00E8D0B600E6CDB300E5CCB200D2B69A00FFF6
      E500E2C8AD00F4EBE2000000000000000000D8E9EC00BD7B7300B5524A00EFD6
      D600EFEFEF00D6D6D600D6D6D600D6D6D600D6D6D600D6D6D600D6D6D600D6D6
      D600EFEFEF00DEBDBD00C65A5A00AD525A00000000000000000000000000B3B3
      E5004646C2004C4CD5006262EA006B6BF3006A6AF3006161E9004C4CD4004646
      C200B3B3E5000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000E3CAB000FBF0
      DD00F7E9D400F5E7D100F4E4CE00F3E2CC00F2E1CC00F2E1CC00DCC2A700E3CA
      B000F5ECE300000000000000000000000000D8E9EC00BD7B7300B5524A00E7D6
      CE00FFF7F700F7EFEF00F7EFEF00F7EFEF00F7EFEF00F7EFEF00F7EFEF00F7EF
      EF00FFF7F700DEBDBD00C65A5A00AD525A000000000000000000000000000000
      0000E5E5F6009393DB005858C7003939BD003939BD005858C7009393DB00E5E5
      F600000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000EBD8C500E4CB
      B200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200E4CBB200F5EC
      E30000000000000000000000000000000000D8E9EC00D8E9EC00AD524A00CEB5
      B500D6D6D600CECECE00CECECE00CECECE00CECECE00CECECE00CECECE00CECE
      CE00D6D6D600CEADAD00A54A4A00D8E9EC000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000D2D2D2009B9B90008E8E
      7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E
      7E008E8E7E009B9B9000D2D2D2000000000000000000D2D2D2009B9B90008E8E
      7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E7E008E8E
      7E008E8E7E009B9B9000D2D2D200000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000E8E8E800DDDDDD00EFEFEF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000F6F6F600E8E8E80099998800FFFF
      FF00FFFFFF00FFFFFF00FFFFFE00FEFEFD00FEFEFC00FDFDFB00FDFDFA00FCFC
      F800FEFEF90099998800E8E8E800F6F6F600F6F6F600E8E8E80099998800FFFF
      FF00FFFFFF00FFFFFF00FFFFFE00FEFEFD00FEFEFC00FDFDFB00FDFDFA00FCFC
      F800FEFEF90099998800E8E8E800F6F6F6000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000C5C5
      C5008F8F8F007D7D7D009C9C9C00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00A7A79600FFFF
      FF00FFFFFF00FEFEFE00FDFDFC00FDFDFB00FCFCF900FBFBF800FAFAF600F8F8
      F400FBFBF600A7A79600FFFFFF00FFFFFF00FFFFFF00FFFFFF00A7A79600FFFF
      FF00FFFFFF00FEFEFE00FDFDFC00FDFDFB00FCFCF900FBFBF800FAFAF600F8F8
      F400FBFBF600A7A79600FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000BDBDBD008F8F
      8F009E9E9E00D3D3D3008A8A8A00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00AFAF9E00FFFF
      FF00FEFEFE00FDFDFC00FDFDFB00FCFCF900FBFBF800FAFAF600F8F8F400F7F7
      F200FBFBF500AFAF9E00FFFFFF00FFFFFF00FFFFFF00FFFFFF00AFAF9E00FFFF
      FF00FF996600FDFDFC00CCCCBB00DDDDCC00DDDDCC00E2E2D700F8F8F400F7F7
      F200FBFBF500AFAF9E00FFFFFF00FFFFFF000000000000000000000000000000
      00000000000000000000000000000000000000000000BBBBBB00979797009292
      9200B5B4B400A2A2A200D0D0D000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00B1B1A000FFFF
      FE00FDFDFC00FDFDFC00FCFCF900FDFDFC00FDFDFC00F8F8F400FDFDFC00F6F6
      F000FAFAF300B1B1A000FFFFFF00FFFFFF00FFFFFF00FFFFFF00B1B1A000FFFF
      FE00FDFDFC00FDFDFB00FCFCF900FBFBF800FAFAF600F8F8F400F7F7F200F6F6
      F000FAFAF300B1B1A000FFFFFF00FFFFFF000000000000000000000000000000
      0000FBFBFB00E1E1E100C5C5C500D3D3D300CECECE00A5A5A50095949500AFAF
      AE0099999900CECECE0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00B4B4A300FEFE
      FD00FDFDFB00FDFDFC00FDFDFC00FDFDFC00FDFDFC00FDFDFC00FDFDFC00F5F5
      EE00FAFAF200B4B4A300FFFFFF00FFFFFF00FFFFFF00FFFFFF00B4B4A300FEFE
      FD0077777700FCFCF900CCCCBB00DBDBCA00D1D1C000D8D8C700D6D6C500F5F5
      EE00FAFAF200B4B4A300FFFFFF00FFFFFF00000000000000000000000000F3F3
      F300B2B2B2008E8D8D00A2A3A3009D9D9D009B9A9A009B9B9B00ADACAC009798
      9800CBCBCB00000000000000000000000000000000000000000000000000CC99
      3300CC993300CC663300CC663300996633009966330099663300996633000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00B8B8A600FEFE
      FC00FCFCF900FDFDFC00FDFDFC00FDFDFC00FDFDFC00FDFDFC00FDFDFC00F4F4
      EC00F9F9EF00B8B8A600FFFFFF00FFFFFF00FFFFFF00FFFFFF00B8B8A600FEFE
      FC00FCFCF900FBFBF800FAFAF600F8F8F400F7F7F200F6F6F000F5F5EE00F4F4
      EC00F9F9EF00B8B8A600FFFFFF00FFFFFF000000000000000000F9F9F900B7B7
      B7009A9A9A00C2C1C100BFBFBF00C6C5C500ADADAD0099979800A2A1A100CFD0
      D00000000000000000000000000000000000000000000000000000000000FFCC
      9900CC993300CC993300CC663300CC6633009966330099663300996633000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00BBBBA900FDFD
      FB00FBFBF800FDFDFC00FDFDFC00FDFDFC00FDFDFC00FDFDFC00FDFDFC00F1F1
      E700F7F7EA00BBBBA900FFFFFF00FFFFFF00FFFFFF00FFFFFF00BBBBA900FDFD
      FB00FF996600FAFAF600C9C9B800D8D8C700D6D6C500DCDCD000F4F4EC00F1F1
      E700F7F7EA00BBBBA900FFFFFF00FFFFFF000000000000000000E5E5E5009595
      9500C6C6C600BCBCBB00BFBFBE00C3C3C300B4B3B300B9B9BA00939393000000
      000000000000000000000000000000000000000000000000000000000000FFCC
      9900FFCC9900CC993300CC993300CC663300CC66330099663300996633000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00BEBEAD00FDFD
      FA00FAFAF600FDFDFC00FDFDFC00F6F6F000F5F5EE00FDFDFC00FDFDFC00ECEC
      DF00F4F4E500BEBEAD00FFFFFF00FFFFFF00FFFFFF00FFFFFF00BEBEAD00FDFD
      FA00FAFAF600F8F8F400F7F7F200F6F6F000F5F5EE00F4F4EC00F1F1E700ECEC
      DF00F4F4E500BEBEAD00FFFFFF00FFFFFF000000000000000000E1E1E1009090
      9000D3D3D200BEBDBD00B9B9B800C4C3C300C2C1C100D2D1D1009E9E9E00EAEA
      EA00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00C0C0AF00FCFC
      F800F8F8F400F7F7F200F6F6F000F5F5EE00F4F4EC00F1F1E700ECECDF00E8E8
      D900F3F3E200C0C0AF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00C0C0AF00FCFC
      F8006E71C200F7F7F200CDCDBC00D4D4C300CACAB900D1D1C000CFCFBE00D6D6
      C500F3F3E200C0C0AF00FFFFFF00FFFFFF000000000000000000DEDEDE009C9C
      9C00EAEAE9007070700058585800C7C7C700BEBDBD00DCDCDC0096969600F3F3
      F300000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00C2C2B100FCFC
      F700F7F7F200F6F6F000F5F5EE00F4F4EC00F1F1E700ECECDF00E8E8D900E6E6
      D500F2F2E100C2C2B100FFFFFF00FFFFFF00FFFFFF00FFFFFF00C2C2B100FCFC
      F700F7F7F200F6F6F000F5F5EE00F4F4EC00F1F1E700ECECDF00E8E8D900E6E6
      D500F2F2E100C2C2B100FFFFFF00FFFFFF000000000000000000EFEFEF00A6A6
      A600D4D4D400A1A1A0008A8A8900C1C1C000CECDCD00CECECE009C9C9C000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00C4C4B400FBFB
      F600F6F6F000F5F5EE00F4F4EC00F1F1E700ECECDF00E8E8D900E6E6D500E5E5
      D400F2F2E100C4C4B400FFFFFF00FFFFFF00FFFFFF00FFFFFF00C4C4B400FBFB
      F60022AA3300F5F5EE00C2C2B100D1D1C000ECECDF00E8E8D900E6E6D500E5E5
      D400F2F2E100C4C4B400FFFFFF00FFFFFF00000000000000000000000000D9D9
      D9008E8E8E00D6D6D600F2F2F200EAEAEA00C4C3C30092929200E4E4E4000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00C7C7B600FAFA
      F400F5F5EE00F4F4EC00F1F1E700ECECDF00E8E8D900E6E6D500A4A49300A4A4
      9300A4A49300A6A69400FFFFFF00FFFFFF00FFFFFF00FFFFFF00C7C7B600FAFA
      F400F5F5EE00F4F4EC00F1F1E700ECECDF00E8E8D900E6E6D500A4A49300A4A4
      9300A4A49300A6A69400FFFFFF00FFFFFF000000000000000000000000000000
      0000DEDEDE00AEAEAE009494940096969600B1B1B100E4E4E400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00C8C8B700FAFA
      F300F4F4EC00F1F1E700ECECDF00E8E8D900E6E6D500E5E5D400B6B6A500FFFF
      FF00C8C8B700ECECE600FFFFFF00FFFFFF00FFFFFF00FFFFFF00C8C8B700FAFA
      F300F4F4EC00F1F1E700ECECDF00E8E8D900E6E6D500E5E5D400B6B6A500FFFF
      FF00C8C8B700ECECE600FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00CACAB900FCFC
      F500F9F9EF00F6F6EA00F4F4E500F3F3E200F2F2E100F2F2E100C2C2B100CACA
      B900ECECE600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00CACAB900FCFC
      F500F9F9EF00F6F6EA00F4F4E500F3F3E200F2F2E100F2F2E100C2C2B100CACA
      B900ECECE600FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FFFFFF00FFFFFF00D8D8CC00CBCB
      BA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00ECEC
      E600FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00D8D8CC00CBCB
      BA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00CBCBBA00ECEC
      E600FFFFFF00FFFFFF00FFFFFF00FFFFFF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FEFEFE00FAFAFA00FAFAFA00FAFAFA00FCFCFC0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000003939390039393900DDDDDD00F4F4F400FDFDFD000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000007300000073000000A66F6F0000000000000000000000
      000000000000000000000000000000000000F9F9F900E9E9E900E5E5E500E5E5
      E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5E500E5E5
      E500E5E5E500E5E5E500E9E9E900F9F9F900F7F7F700F1F1F100EDEDED00EDED
      ED00EDEDED00EDEDED00EDEDED00EDEDED00EDEDED00EDEDED00EDEDED00EDED
      ED00EFEFEF00F1F1F100F9F9F900FDFDFD000000000000000000000000000000
      00000000000039393900525252004E4E4E0039393900D6D6D600F2F2F200FDFD
      FD00000000000000000000000000000000000000000000000000000000000000
      00000000000073000000A8554300AFA39F0073000000C7AAAA00000000000000
      000000000000000000000000000000000000759DB1004A8FB2004A8FB2004A8F
      B2004A8FB2004A8FB2004A8FB2004A8FB2004A8FB2004A8FB2004A8FB2004A8F
      B2004A8FB2004A8FB2004A8FB200759DB100CBD8E0005C99B5004F96B7004D93
      B6004C92B6004C92B6004C92B6004C92B6004C92B6004C92B6004C92B6004C92
      B6004D93B6005791AE00E1E5E700FBFBFB000000000000000000000000000000
      0000393939007B7B7B004E4E4E00D8D8D800B0B0B00039393900D6D6D600F2F2
      F200FDFDFD000000000000000000000000000000000000000000000000000000
      000073000000B7681600A0330000C3B6B300BEB9B60073000000C7AAAA000000
      000000000000000000000000000000000000579EC20086CFF00082CBED0082CB
      ED0082CBED0082CBED0082CBED0082CBED0082CBED0082CBED0082CBED0082CB
      ED0082CBED0082CBED0086CFF000579EC200C5DCE8007BC3DD007CC8E70081CB
      EB007EC9E9007EC9E9007EC9E9007EC9E9007EC9E9007EC9E9007EC9E9007EC9
      E9007EC9E9006EB8DA00B9D1DE00000000000000000000000000000000003939
      3900949494004E4E4E00F3F3F300EFEFEF00D8D8D800B0B0B00039393900D6D6
      D600F2F2F200FDFDFD0000000000000000000000000000000000FEFEFE007300
      0000C4812700AC4D0000A3390000AFA5A000F7F6F600D6D4D20073000000C7AA
      AA000000000000000000000000000000000065ACD10086CFEE007DC8E8007DC8
      E8007DC8E8007DC8E8007DC8E8007DC8E8007DC8E8007DC8E8007DC8E8007DC8
      E8007DC8E8007DC8E80086CFEE0065ACD100C9E0EC0088CEE9006EBFDE0086D0
      ED0081CCEA0081CCEA0081CCEA0081CCEA0081CCEA0081CCEA0081CCEA0081CC
      EA0081CCEA0083CEEC008EBED500F9FBFC00000000000000000039393900A9A9
      A9004E4E4E00E5E5E500BFBFBF00EEEEEE00EFEFEF00D8D8D800B0B0B0003939
      3900D5D5D500F1F1F100FCFCFC000000000000000000FAF7F70073000000DCB3
      7D00C17B1A00AF530000A63F0000A83F0000F6E9DB00FAFAFA00F3F3F3007300
      0000C7AAAA0000000000000000000000000069B1D5008AD3F00082CCEB0082CC
      EB0082CCEB0082CCEB0082CCEB0082CCEB0082CCEB0082CCEB0082CCEB0082CC
      EB0082CCEB0082CCEB008AD3F00069B1D500CBE2EE008BD2EA0074C5E40088D3
      EE0088D1EE0088D1EE0088D1EE0088D1EE0088D1EE0088D1EE0088D1EE0088D1
      EE0088D1EE0089D3EE0085C1DC00E5F0F600FCFCFC0039393900C6C6C6004E4E
      4E00E5E5E500B6B6B600A1A1A10090909000E4E4E400EFEFEF00D8D8D800B0B0
      B00039393900D5D5D500E4E4E400FAFAFA00F2E9E70073000000EBD5B400DFBB
      8600D3A15B00C0791600A9470000A0310000DFA36500DBB17900FAFAFA00F8F8
      F80073000000C7AAAA0000000000000000006DB5D9008FD7F20087D0ED0087D0
      ED0087D0ED0087D0ED0087D0ED0087D0ED0087D0ED0087D0ED0087D0ED0087D0
      ED0087D0ED0087D0ED008FD7F2006DB5D900CDE4EF008FD5EB0081D0EC0085D4
      EE0092D9F30091D8F20091D8F20091D8F20091D8F20091D8F20091D8F20091D8
      F20091D8F20091D8F2008AD0E900CDE5F10039393900DDDDDD004E4E4E00E5E5
      E500CCCCCC00B4B4B400A3A3A3009696960083838300D4D4D400EFEFEF00D8D8
      D800B0B0B00039393900D4D4D400DBDBDB0073000000F6EDDF00EDD9BB00E3C3
      9600D8A96C00CD914100B76500009F310000E1AD7900B25B0000BC710600F1F1
      F000FAFAFA0073000000C7AAAA00FBF9F90071B8DC0094DBF4008DD5F0008DD5
      F0008DD5F0008DD5F0008DD5F0008DD5F0008DD5F0008DD5F0008DD5F0008DD5
      F0008DD5F0008DD5F00094DBF40071B8DC00CDE4F00091D8EE008DD9F2007BCF
      EC009CE2F70098DEF60098DEF60098DEF60098DEF60098DEF60098DEF60098DE
      F60098DEF60098DEF60090D7EF00AFD9ED0039393900DDDDDD00C9C9C9004E4E
      4E00E5E5E500D0D0D000BABABA00ABABAB009D9D9D0081818100E1E1E100EFEF
      EF00D8D8D800B0B0B00039393900E4E4E40073000000F8F1E500F0DFC800E7CB
      A700DCB37C00D19B5200C7872A00AA450000DA9D5B00D19B4E00AF510000A139
      0000DDDBD800FAFAFA0073000000DFCFCF0075BCE00099E0F60092DAF30092DA
      F30092DAF30092DAF30092DAF30092DAF30092DAF30092DAF30092DAF30092DA
      F30092DAF30092DAF30099E0F60075BCE000CFE6F20094DAF0009BE3F90071C8
      E9008DDCF1008CDDF3008CDCF3008BDCF3008BDCF2008BDBF2008ADBF2008ADA
      F2008ADAF20086D7EF0098DEF20096D1EA0039393900A5A5A500DDDDDD00CECE
      CE004E4E4E00DDDDDD00CECECE00C7C7C700C1C1C100BABABA009D9D9D00D5D5
      D500EFEFEF00D8D8D80039393900E3E3E30073000000E7CCA800F3E5D200EBD3
      B400DDB78300D5A55F00CB8F3B00C0791000B9630000DEB98900CB904100BC79
      4200DCC0A500E7E5E30073000000E6DADA0079C0E3009FE5F90098DFF60098DF
      F60098DFF60098DFF60098DFF60098DFF60098DFF60098DFF60098DFF60098DF
      F60098DFF60098DFF6009FE5F90079C0E300D1E7F30097DCF100A3E8FB0093DD
      F40084D3EF0081D3F0007FD1EF007CCFEE007ACCEC0078CAEB0076C8EA0073C6
      E90073C6E90066B8DE00D5EBF600DDEFF8000000000039393900393939003939
      390039393900C4C4C400D8D8D800DBDBDB00CCCCCC00C1C1C100B3B3B300B2B2
      B200C3C3C30039393900D6D6D600F3F3F3000000000073000000730000007300
      00007300000073000000C7883300C98D3700B9690000AA501200DAB58200F7EF
      E600EBDAC500E0C7AE0073000000DAC9C9007DC2E600A3E9FB009DE3F9009DE3
      F9009DE3F9009DE3F9009DE3F9009DE3F900A3E9FA00A3E9FA00A3E9FA00A3E9
      FA00A3E9FA00A3E9FA00A6ECFB007DC2E600D1E8F40098DDF300A5EAFC00A3E8
      FB00A1E6FA009FE4F9009CE2F80099E0F60096DDF50093DBF30090D8F2008ED6
      F0008DD5F0007DC4E50000000000000000000000000000000000000000000000
      00000000000039393900DDDDDD00DEDEDE00DDDDDD00D4D4D400C9C9C900CCCC
      CC0039393900D6D6D600F2F2F200FDFDFD000000000000000000000000000000
      0000000000000000000073000000C0834B00B2663C007300000073000000FCF6
      F200F5EDE100EBD9C30073000000DAC9C9007FC5E800A8EDFD00A2E7FB00A2E7
      FB00A2E7FB00A2E7FB00A2E7FB00ABF0FD008CD0ED0081C5E70081C5E70081C5
      E70081C5E70081C5E70081C5E7007FC5E800D3E9F5009CE0F400AAEEFE00A9ED
      FD00A7ECFC00A5EAFB00A1E6F9009ADFF60096DDF50093DBF40091D8F2008FD6
      F1008ED5F10080C6E6000000000000000000FEFEFE0000000000000000000000
      0000000000000000000039393900DDDDDD00DDDDDD00DDDDDD00D2D2D2003939
      3900D6D6D600F5F5F500FEFEFE00000000000000000000000000000000000000
      0000000000000000000000000000730000007300000000000000F4EEED007300
      0000FAF6F200F3E8DA0073000000D3BDBD0083C8EB00AEF3FF00ABF0FE00ABF0
      FE00ABF0FE00ABF0FE00AEF3FF008FD3EF008FD3EF00ABF0FE00ABF0FE00ABF0
      FE00ABF0FE00ABF0FE00AEF3FF0083C8EB00E9F4FA00AFD9EE00A7D5EC00A7D5
      EC00A7D5EC00A7D5EC00A1D2EB008DCDEB00D5ECF600CFE6EB00CDD8BF00D2C1
      72008BCCEA00B1D9EE0000000000000000000000000000000000000000000000
      000000000000000000000000000039393900DDDDDD00DDDDDD0039393900E1E1
      E100F7F7F700FDFDFD0000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000F7F2
      F20073000000E7D1B0007300000000000000A3D7F10085CAEC0085CAEC0085CA
      EC0085CAEC0085CAEC0085CAEC0085CAEC00FEFEFD00F8F8F300F0F0E600E9E9
      DB00FEC94100F4B62E0085CAEC00A3D7F1000000000000000000000000000000
      00000000000000000000F9FCFD00C9E5F30081C6E80081C6E80081C6E80081C6
      E800C9E5F300F9FCFD0000000000000000000000000000000000000000000000
      000000000000FDFDFD0000000000000000003939390039393900F1F1F100FCFC
      FC00FDFDFD000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000007300000000000000000000000000000000000000000000000000
      0000000000000000000000000000D5EDF90087CBEE0087CBEE0087CBEE0087CB
      EE0087CBEE0087CBEE00D5EDF900000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FAFAFA000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000F5EFEE000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000874A2000874A2000874A2000874A
      2000874A20000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FCFCFC00FAFAFA00FDFDFD000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000FCFCFC00FAFAFA00FDFDFD000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FDFD
      FD0000000000FDFDFD000000000000000000874A2000BEA59200B89C8600B89C
      8600874A2000874A20000000000000000000000000000000000000000000858A
      8800858A8800858A880000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000072727100ADACAC00F9F9F9000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000424241009C9B9A00F8F8F8000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000D3D3D3007D7D
      7D006D6D6D0070707000E9E9E90000000000874A2000CCBBAD00A7917D00B595
      7A00AE8B7200874A2000874A2000000000000000000000000000858A8800858A
      88009EA1A000858A88000000000000000000E8E6E500AA9C8700AA9C8700AA9C
      8700AA9C8700AA9C8700AA9C8700AA9C8700AA9C8700AA9C8700AA9C8700A99B
      8600625F5B00908F8F00DBD9D600E6E3DE000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FEFE
      FE00424242007B7A7A0000000000FCFCFC000000000000000000000000000000
      00000000000000000000000000000000000000000000E9E9E90070707000EAEA
      EA00E2E2E200E0E0E00081818100FBFBFB00874A2000CCBBAD00A48D7800A28A
      7400B4957A00B3937C00874A20000000000000000000858A8800858A8800D2D3
      D400C2C3C400858A88000000000000000000AA9C8700F4F3F400F4F3F400F4F3
      F400F4F3F400F4F3F400F4F3F400F4F3F400F4F3F400F4F3F400F4F3F4007170
      700088888700DDDCDB00EFEFF000AA9C87000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000004444
      42006F6F6D00F3F2F000FEFEFE00000000000000000000000000000000000000
      000000000000EDEDED00A1A1A100F7F7F7000000000071717100ECECEC00F4F4
      F400B4B4B400EDEDED00E4E4E40098989800874A2000874A2000D4C8BD00A48D
      7800A48D7800BEA59200874A200000000000858A8800858A8800E2E3E400C2C4
      C600858A8800858A88000000000000000000AA9C8700F8F8F800000000000000
      0000000000000000000000000000000000000000000000000000747474007E7D
      7D00D1D1D000FCFCFC00F5F5F500AA9C87000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000444444005F5D
      5D00E1DFDE00FEFEFE000000000000000000000000000000000000000000F5F5
      F500707070006B6B6B00D9D9D9006D6D6D006D6D6D0064646400E2E2E2006666
      6600C8C8C8008A8A8A00F4F4F4009696960000000000874A2000874A2000CCBB
      AD00CCBBAD00B3937C00874A2000858A8800858A8800D3D3D400BDBEBF00858A
      8800858A8800000000000000000000000000AA9C8700F8F8F800000000000000
      0000F9F9F900E5E6E600F4F4F400FEFEFE00000000007878780075747300D0CF
      CE00F9F9F800FDFDFC00F5F5F400AA9C87000000000000000000000000000000
      0000F7F7F700DFDFDF00F0F0F000FDFDFD000000000048484800504F4D00DCDA
      D900000000000000000000000000000000000000000000000000ECECEC008D8D
      8D00E0E0E000C7C7C700C6C6C600C1C1C100C5C5C50084848400D9D9D900D1D1
      D1006E6E6E00FEFEFE00F4F4F400B4B4B4000000000000000000874A2000874A
      2000874A2000874A200085837D00AAADAD00C8C9CA00BDBEBF00858A8800858A
      880000000000000000000000000000000000AA9C8700F9F9FA00DEDEDE009998
      98009B938F00A8989300A1969100A8A6A6007B7C7C007D7D7C00DADAD900FBFB
      FB00FDFDFD00FEFEFE00F5F5F500AA9C87000000000000000000DEDEDE007C7A
      7A008D807900A288800097857D0099959500515151005B5B5A00E7E5E400FEFE
      FE000000000000000000000000000000000000000000F5F5F5008F8F8F000000
      0000FCFCFC00FEFEFE00EDEDED00F7F7F700DBDBDB00B5B5B5009B9B9B00CFCF
      CF00EAEAEA00F6F6F600B3B3B300000000000000000000000000000000000000
      00000000000000000000B5B7B800858A8800B7B8B900858A8800858A88000000
      000000000000000000000000000000000000AA9C8700E2E2E200ACA6A500FCE1
      D500FBECE400FBEFE600FBEDE400F9DBCE00A09A9800BDBDBD00000000000000
      00000000000000000000F6F6F600AA9C870000000000E7E7E700A3999600FFFD
      EA00000000000000000000000000FFF4DF00958C8800ACADAD00000000000000
      000000000000000000000000000000000000000000008B8B8B00DFDFDF00F5F5
      F500DADADA009A9A9A00C3C3C30091919100DADADA00DADADA00C6C6C6005E5E
      5E007A7A7A0086868600000000000000000000000000858A8800858A8800858A
      8800858A8800858A8800D0D1D200A3A4A400858A8800858A8800000000000000
      000000000000000000000000000000000000A99B86009B9A9A00FADFD500FBF1
      E800FCEDDA00F7EDE700F7EEE300FCF2E800F6D3C300A6A3A200FDFDFD000000
      00000000000000000000F6F5F500AA9C8700FDFDFD007F7E7E00FFF9E8000000
      0000FFFFEB00FFFFFD000000000000000000FFE2C80095908D00FCFCFC000000
      000000000000000000000000000000000000E6E6E6006E6E6E00DEDEDE00F9F9
      F900979797000000000000000000000000008A8A8A00F1F1F100D0D0D0006F6F
      6F00FDFDFD00000000000000000000000000858A8800858A8800F3F3F300EFF0
      F000EDEEEE00EAECEC00B6B9BA00858A8800858A8800858A8800858A88000000
      000000000000000000000000000000000000A5988400ACA5A300FFF7F300FBF2
      E800FAF0E100FAF2EE00F7EEE500FFFEFE00FBF1EB00B0A6A200EFEFEF000000
      00000000000000000000F5F4F400AA9C8700F4F4F400A094900000000000FFFF
      FC00FFFFEE0000000000FFFFFD000000000000000000A7979000E9E9E9000000
      00000000000000000000000000000000000095959500FBFBFB00DDDDDD00C6C6
      C600C8C8C800000000000000000000000000B2B2B200D5D5D500D9D9D900EFEF
      EF00ACACAC00000000000000000000000000858A8800F5F6F600A9ACAB00858A
      8800F7F7F700E2E3E500AAADAD00ECEEEE0000000000858A8800858A8800858A
      8800000000000000000000000000000000009D907E00B6AEAB00FFFDFA00FAF2
      EE00FAF2EE00F9F0EA00FAF2EE00FAF2EE00FCF6F100B3A9A500E5E5E5000000
      00000000000000000000F7F7F700AA9C8700DFDFDF00AEA09C00000000000000
      000000000000FFFFFB00000000000000000000000000A5959000DDDCDC000000
      000000000000000000000000000000000000E6E6E60092929200E3E3E300E8E8
      E8006B6B6B000000000000000000000000005F5F5F00F6F6F600E1E1E1009999
      9900EFEFEF00000000000000000000000000858A8800858A880000000000858A
      8800858A8800FAFAFA00858A8800000000000000000000000000858A8800858A
      8800878C8A00000000000000000000000000A5988400A4A09F00FCF9F600FCF6
      F000F9F4EE00FAF2EE00F8F2EB00FEFCF900FEFBFA00A59F9C00F2F2F2000000
      00000000000000000000F8F8F800AA9C8700F4F4F4008F878500000000000000
      0000FFFFFD00000000000000000000000000000000008F847F00EEEEEE000000
      000000000000000000000000000000000000000000007E7E7E00EBEBEB00F2F2
      F200A0A0A0005C5C5C008181810054545400BCBCBC00FEFEFE00F3F3F3008585
      8500000000000000000000000000000000000000000000000000000000000000
      0000858A8800EEF0F000858A880000000000000000000000000000000000858A
      8800F0F0F000858A88000000000000000000A99B860088817600C4BBB100EFED
      E900D9D3C900D9CCBF00DED6CB00DDD7CF00E4DFD80086807A00AA9C8700AA9C
      8700AA9C8700AA9C8700AA9C8700AA9C8700FDFDFD009B9B9B00F0E9E7000000
      0000000000000000000000000000000000000000000083808000FDFDFD000000
      00000000000000000000000000000000000000000000DBDBDB00B0B0B000E2E2
      E200F3F3F300E3E3E300C4C4C400F1F1F100FEFEFE00E8E8E800A2A2A200DFDF
      DF0000000000000000000000000000000000000000000000000000000000858A
      8800E9EBEC00858A8800858A8800000000000000000000000000000000000000
      0000858A8800EEEFEF00858A880000000000AA9C8700E9E0DB00A19D9B00F2F0
      F000FBFAFA00F9F5F300FAF7F500F8F5F3009B959200D7D0CB00EFE6E000AA9C
      8700EFE6E000AA9C8700EFE6E000AA9C870000000000F6F6F6008F8D8F000000
      00000000000000000000000000000000000085807F00DCDCDC00000000000000
      0000000000000000000000000000000000000000000000000000F4F4F4009E9E
      9E00FCFCFC00FEFEFE00F2F2F200FEFEFE00FEFEFE00A3A3A300F4F4F4000000
      000000000000000000000000000000000000000000000000000000000000858A
      8800858A8800858A880000000000000000000000000000000000000000000000
      000000000000858A8800858A880000000000E7E3DD00AA9C8700A79A8600928A
      80007B746A00847C7100837A7000847E7600A1968500AA9C8700AA9C8700AA9C
      8700AA9C8700AA9C8700AA9C8700E5E0DA000000000000000000F0F0F000A9A9
      A9008483830092908F00908E8E008B8B8C00DDDCDB0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000DBDB
      DB00BDBDBD0094949400FCFCFC0094949400C0C0C000E5E5E500000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FDFD
      FD00F6F6F600E9E8E800F4F4F400FCFCFC000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FDFD
      FD00F4F4F400E2E2E200F1F1F100FBFBFB000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E6E6E600E8E8E800E7E7E700FEFEFE0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FEFEFE00FAFAFA00FAFAFA00FAFAFA00FCFCFC0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FCFAF900E5D1B900DDB68900E1B27D00E1B27D00DDB68900E5D1B900FCFA
      F900000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000626262006262
      6200616161006262620062626200626262006262620061616100626262006161
      6100616161006262620000000000000000000000000000000000000000000000
      000000000000000000007300000073000000DDDDDD00F4F4F400FDFDFD000000
      000000000000000000000000000000000000000000000000000000000000E2CD
      BC00D48A5200D3763B00DE875A00EF9B7400F19D7700E18B5F00D3773F00CF87
      4F00E2CDBB00000000000000000000000000C9C3B700C5BFB700BFB7AF00BBB4
      A900BBB2A700B9B0A500B9B0A400B8AFA300BBB0A500BEB2A700BFB3AA00BFB4
      AA00BFB6AC00C2B6B000C1B7B100CAC3B9000000000000000000ABABAB00F9F9
      F900E8E8E800E8E8E800E4E4E400E0E0E000D9D9D900D7D7D700D1D1D100CECE
      CE00C7C7C7006262620000000000000000000000000000000000000000000000
      00000000000073000000A54B00009D3B000073000000D6D6D600F2F2F200FDFD
      FD00000000000000000000000000000000000000000000000000D7B7A100CB69
      2B009E3E10008B300B00A1431C00B3532B00B6552D00A6482100903510009A3A
      0E00C1612500D7B6A0000000000000000000C5BBA900D7CBBE00D7CBBE00D7CB
      BE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CB
      BE00D7CBBE00D7CBBE00D7CBBE00CCC1AF000000000000000000AAAAAA000000
      0000E8E8E800E6E6E600E2E2E200E0E0E000DADADA00D7D7D700CFCFCF00CECE
      CE00C7C7C7006262620000000000000000000000000000000000000000000000
      000073000000BF8337009D3B0000D8D8D800B0B0B00073000000D6D6D600F2F2
      F200FDFDFD0000000000000000000000000000000000E0CBBF00C2622900822C
      0700751A000080250100A7644900C99A8600CA9B8700A9664A0081260200771B
      00007F290500B4572200E0CABF0000000000C5B8A6000000000000000000D7CB
      BE000000000000000000D7CBBE000000000000000000D7CBBE00000000000000
      0000D7CBBE000000000000000000CDC2AD000000000000000000AAAAAA000000
      0000ECECEC00E8E8E800E4E4E400E2E2E200DEDEDE00D7D7D700D1D1D100CECE
      CE00C7C7C7006161610000000000000000000000000000000000000000007300
      0000CC9B5D009D3B0000F3F3F300EFEFEF00D8D8D800B0B0B00073000000D6D6
      D600F2F2F200FDFDFD000000000000000000FAF8F700C2754C007E320E005D13
      000081371D00DFC7BE0000000000FEFFFF00FEFEFF0000000000DFC8BF008337
      1D0061140000742B0900B66D4600FAF8F700C4B8A3000000000000000000D7CB
      BE000000000000000000D7CBBE000000000000000000D7CBBE00000000000000
      0000D7CBBE000000000000000000CEC3AD000000000000000000AAAAAA000000
      0000F5F5F500E8E8E800E4E4E400E0E0E000D9D9D900D7D7D700D1D1D100CECE
      CE00C8C8C800626262000000000000000000000000000000000073000000D6AF
      7C009D3B0000F3E7D800E1C39D00EEEEEE00EFEFEF00D8D8D800B0B0B0007300
      0000D5D5D500F1F1F100FCFCFC0000000000DDC4BC00A04C1F004A1300005D1D
      0900E0D1CB00FEFEFE00AB7C6A0080341B0081341A00AB7C6A00FDFEFE00E0D2
      CB005F1E09004A1200008B3F1700DCC3BB00C6BAA500D7CBBE00D7CBBE00D7CB
      BE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CB
      BE00D7CBBE00D7CBBE00D7CBBE00D0C4AE000000000000000000AAAAAA000000
      0000FBFBFB00E9E9E900E4E4E400E0E0E000D9D9D900D7D7D700CFCFCF00CCCC
      CC00C5C5C500626262000000000000000000FCFBFB0073000000E4CAA8009D3B
      0000F3E7D800DDBB9000D3A76F00CB975500E4E4E400EFEFEF00D8D8D800B0B0
      B00073000000D5D5D500E5E3E300FAFAFA00C6948200703213002D0200008766
      5700000000009F7666005A0800006A1A00006C1B00005F0A0000A47865000000
      00008A6757003002000059250C00C0907F00C6B9A4000000000000000000D7CB
      BE000000000000000000D7CBBE000000000000000000D7CBBE00000000000000
      0000D7CBBE000000000000000000CCC0AA000000000000000000AAAAAA000000
      000000000000F3F3F300E2E2E200DEDEDE00D7D7D700D3D3D300CECECE00C7C7
      C700C5C5C50062626200000000000000000073000000EFDFCB009D3B0000F3E7
      D800E7CFB100DCB98D00D4A97200CE9D5F00C58B4100D4D4D400EFEFEF00D8D8
      D800B0B0B00073000000D4D4D400E0D6D600B97C670051220C0036060000B79D
      920000000000914424008B320F00913610009237110090361100984827000000
      0000C4A395005E16010061260B00B1776400C3B6A1000000000000000000D7CB
      BE000000000000000000D7CBBE000000000000000000D7CBBE00000000000000
      0000D7CBBE000000000000000000C9BEA9000000000000000000AAAAAA000000
      0000FDFDFD00FBFBFB00EEEEEE00DEDEDE00CFCFCF00CECECE00C5C5C500C1C1
      C100BEBEBE0061616100000000000000000073000000EFDFCB00E6CDAD009D3B
      0000F3E7D800E9D3B700DFBF9600D8B17F00D1A36900C4893E00E1E1E100EFEF
      EF00D8D8D800B0B0B00073000000E4E4E400BB7D6900974319008B2D0600CFA2
      8F0000000000B2644300A1421900B1603E00B1613F00A2421900B36645000000
      0000D1A4910097350B00A6481B00B77A6600C4B6A200D7CBBE00D7CBBE00D7CB
      BE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CBBE00D7CB
      BE00D7CBBE00D7CBBE00D7CBBE00C9BDAA000000000000000000AAAAAA000000
      000000000000FDFDFD00F4F4F400ECECEC00D1D1D100C5C5C500BEBEBE00BCBC
      BC00B7B7B70061616100000000000000000073000000D4AA7700EFDFCB00E8D1
      B4009D3B0000EFDFCB00E8D1B400E5CBAA00E2C5A000DFBF9600D1A36900D5D5
      D500EFEFEF00D8D8D80073000000E3E2E200C28E8400C0623000A3451E00BF79
      5B0000000000DEB9A900B1512900EFDBD300EFDBD200B4573000E4C5B700FEFF
      FF00C0785A00A74A2300B75C2F00BE8C8200C6B7A1000000000000000000D7CB
      BE000000000000000000D7CBBE000000000000000000D7CBBE00000000000000
      0000D7CBBE000000000000000000CABDAA000000000000000000AAAAAA000000
      0000000000000000000000000000F9F9F900E8E8E800D9D9D900B3B3B300ADAD
      AD00AAAAAA006262620000000000000000000000000073000000730000007300
      000073000000E3C8A500EDDBC400EEDDC800E7CFB100E2C5A000DBB88C00D9B7
      8B00E3C7A40073000000D6D6D600F3F3F300D7BDBD00D3794A00C96F4800C86A
      4300EDC9BA0000000000FEA58100F7E4DD00F7E6DF00FEA5810000000000E9BE
      AB00CB6C4500CC724C00CB724600D6BCBC00C6B8A1000000000000000000D7CB
      BE000000000000000000D7CBBE000000000000000000D7CBBE00000000000000
      0000D7CBBE000000000000000000CCBEAA000000000000000000AAAAAA000000
      00000000000000000000FDFDFD00F9F9F900E6E6E600DEDEDE00BABABA00A6A6
      A600A3A3A3006262620000000000000000000000000000000000000000000000
      00000000000073000000EFDFCB00EFE0CC00EFDFCB00EAD6BE00E5CDAD00E7CF
      B10073000000D6D6D600F2F2F200FDFDFD00FAF8F800C0785F00F39C7100E88B
      6400E9906A00F8C4AF00FEA58100FCEBE400FCECE500FEA58100F7BEA600EA8D
      6500EB8E6800F49C7200BB745D00FAF8F800BFB19B00C3B8AD00C3B8AD00C2B8
      AB00C2B8AA00C1B8AA00C1B7AA00C1B6A900C1B6A800C1B6A900C1B6A900C2B8
      AB00C3B9AC00C3B9AD00C4BAAD00C3B59F000000000000000000AAAAAA000000
      00000000000000000000FCFCFC00F3F3F300E4E4E400D9D9D900CFCFCF00B5B5
      B5009F9F9F00616161000000000000000000FEFEFE0000000000000000000000
      0000000000000000000073000000EFDFCB00EFDFCB00EFDFCB00EAD5BB007300
      0000D6D6D600F5F5F500FEFEFE000000000000000000D9C2C200E2906800FFB6
      9100FEA88500FDA27E00FEA68200FFEFE900FFEFE900FEA58100FDA48000FFAA
      8800FFB79300DD8A6600D8C2C20000000000C3B6A400E4D8C800E4D8C800E2D5
      C500E1D4C500DCD0C200E1D5C500E0D3C400E2D5C800DDD1C400E1D4C500E3D7
      C700DDD0C100E1D4C500E1D5C500C5B8A5000000000000000000AAAAAA000000
      00000000000000000000F9F9F900F4F4F400E0E0E000D9D9D900626262006161
      6100626262006161610000000000000000000000000000000000000000000000
      000000000000000000000000000073000000EFDFCB00EFDFCB0073000000E1E1
      E100F7F7F700FDFDFD0000000000000000000000000000000000C9A7A400DF9B
      7900FFCEB200FFC8AF00FFC2A900FFCCB700FFCCB700FFC2A900FFC9B100FFCF
      B400DB977800C8A5A3000000000000000000C0B5A600F0E6DC00F0E6DC00ECE2
      D500EBE0D300ECE1D300EBDCCF00EBDDCE00EBDDCC00EBDDCE00EBDFCF00EDE0
      D100EDE3D600EDE4DB00EFE7DE00C4BAA9000000000000000000AAAAAA000000
      00000000000000000000F9F9F900F1F1F100DADADA00D9D9D900AAAAAA00AFAF
      AF00626262000000000000000000000000000000000000000000000000000000
      000000000000FDFDFD0000000000000000007300000073000000F1F1F100FCFC
      FC00FDFDFD00000000000000000000000000000000000000000000000000D7C0
      BF00C2877600ECB89E00FFD6C000FFDBC700FFDCC700FFD6C100EAB69E00BF85
      7500D7BFBF00000000000000000000000000C8C0B400C3B8AD00C3B8AD00C2B8
      AB00C2B8AA00C1B8AA00C1B7AA00C1B6A900C1B6A800C1B6A900C1B6A900C2B8
      AB00C3B9AC00C3B9AD00C4BAAD00CAC3B7000000000000000000AAAAAA000000
      00000000000000000000F3F3F300ECECEC00D5D5D500D9D9D900AAAAAA006262
      6200000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FAFAFA000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FCFBFB00D6BDBD00C1969100BE8D8500BE8D8400C0969000D6BDBD00FCFB
      FB00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000ABABAB00ABAB
      AB00AAAAAA00AAAAAA00AAAAAA00AAAAAA00AAAAAA00AAAAAA00626262000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000800000000100010000000000000400000000000000000000
      000000000000000000000000FFFFFF00FF000000000000008000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000070000000000000007000000000000
      0007000000000000000700000000000000070000000000000007000000000000
      0007000000000000800F000000000000FFFFF8008001FF00C7E1E00080018000
      83C10000E007000081810000C00300008001C00080010000C003C00080010000
      C007C00080010000F00FC00080010000F00FC00080010007E007C00080010007
      C003C000802100078001C004800100078181C001C00300078381C003E0070007
      87E3C003F00F0007FFFFC003FFFF800F00008001800180010000000000008001
      000000000000E007000000000000C0030000000000008001EAAE000000008181
      B5510000000083C1B5560000000087E1CD540000000080E1B5540000000080C1
      B414000000008181EFE80000000080010FC00000000080030FE0000000008007
      000000000000900F000000000000FFFFFFFF000000008001FFFF000000008001
      FFFFC0030020E007FFFFC0030020C003FFFFC00300208001E01FC00100008001
      E01FC00000008001E01FC00000008001FFFFC00000008001FFFFC00300008401
      FFFFC00300008E71FFFFC00300008421FFFFC0030000C003FFFFC0030000E007
      FFFFC0070000F00FFFFFC00F0000FFFF80018001FFF1FFFF00000000FFE1FFFF
      00000000FFC1FFFF00000000FF81FFFF00000000F003FFFF00000000E007E01F
      00000000C00FE01F00000000C01FE01F00000000C00FFFFF00000000C00FFFFF
      00000000C01FFFFF00000000E01FFFFF00000000F03FFFFF00000000FFFFFFFF
      00000000FFFFFFFF00000000FFFFFFFFF07FFFFFFFFFFFFFFC1FFC7F00000000
      F80FF83F00000000F007F01F00000001E003C00F00000000C001800700000000
      0000000300000000000000000000000000000000000000000000000000000000
      8000800000000003F800FC00000000037C01FE4000000003FE03FFE10000FC03
      FB07FFFBFE01FFFFFFDFFFFEFFFFFFFF07FFFFF8FFF8FFEB03E3FFF8FFF8FFC1
      01C30000FFE2FF8001830000FFE1F88001033FC0FFC3E00080073080F08FC000
      C00F0000C00F9001FC1F003C8E3F8003803F001C131F0707001F001C259F0707
      008F001C3B9F070721C7001C379F800FF1E300001F9F800FE1F100009F3FC01F
      E3F90000C07FE03FFFFFE0FFE0FFF87FF07FF00FFFFFC003FC1FE0070000C003
      F80FC0030000D003F00780016DB6D003E00302406DB6D003C00100000000D003
      000008106DB6D803000008106DB6D003000008100000D803000008006DB6DE03
      800004206DB6DC03F80000000000DC037C0180010000DC03FE03C0030000DC07
      FB07E0070000DC0FFFDFF00FFFFFC01F00000000000000000000000000000000
      000000000000}
  end
  object MainMenu1: TMainMenu
    Images = ImageListMain
    Left = 120
    Top = 520
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N_FileOpen: TMenuItem
        Action = ActionFileOpen
      end
      object N21: TMenuItem
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        object N22: TMenuItem
          Action = ActionSaveConfig
        end
        object N23: TMenuItem
          Action = ActionSaveConfigSave
        end
        object N30: TMenuItem
          Action = ActionSaveLastDepotConfig
        end
        object N24: TMenuItem
          Caption = '-'
        end
        object Records1: TMenuItem
          Action = ActionSaveDataFile
        end
        object Blob1: TMenuItem
          Action = ActionSaveBlobFile
        end
        object Index1: TMenuItem
          Action = ActionSaveIndexFile
        end
        object N26: TMenuItem
          Caption = '-'
        end
        object XML1: TMenuItem
          Action = ActionExportTableToXML
        end
      end
      object N_Exit: TMenuItem
        Action = ActionExit
      end
    end
    object N2: TMenuItem
      AutoLineReduction = maAutomatic
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      object N3: TMenuItem
        Action = ActionDebugMessages
      end
      object N6: TMenuItem
        Action = ActionBlobView
        object N8: TMenuItem
          Caption = #1056#1077#1078#1080#1084' '#1088#1072#1079#1073#1086#1088#1072
          OnClick = ActionBlobViewExecute
          object N16: TMenuItem
            Action = ActionBlobParseModeDetect
            AutoCheck = True
          end
          object N17: TMenuItem
            Caption = '-'
          end
          object N18: TMenuItem
            Action = ActionBlobParseUnzip
            AutoCheck = True
          end
          object N2Unzip21: TMenuItem
            Action = ActionBlobParseUnzip2
            AutoCheck = True
          end
          object ActionBlobParseContainer1: TMenuItem
            Action = ActionBlobParseContainer
            AutoCheck = True
          end
          object N19: TMenuItem
            Action = ActionBlobParseValueStorage
            AutoCheck = True
          end
          object N110: TMenuItem
            Action = ActionBlobParseValue1C
            AutoCheck = True
          end
        end
        object N4: TMenuItem
          Caption = #1056#1077#1078#1080#1084' '#1087#1088#1086#1089#1084#1086#1090#1088#1072
          OnClick = ActionBlobViewExecute
          object N9: TMenuItem
            Action = ActionBlobViewModeDetect
            AutoCheck = True
          end
          object N5: TMenuItem
            Caption = '-'
          end
          object N10: TMenuItem
            Action = ActionBlobViewANSI
            AutoCheck = True
          end
          object N11: TMenuItem
            Action = ActionBlobViewUTF8
            AutoCheck = True
          end
          object N12: TMenuItem
            Action = ActionBlobViewUTF16
            AutoCheck = True
          end
          object N13: TMenuItem
            Action = ActionBlobViewMedia
            AutoCheck = True
          end
          object N14: TMenuItem
            Action = ActionBlobViewWEB
            AutoCheck = True
          end
          object N15: TMenuItem
            Action = ActionBlobViewHEX
            AutoCheck = True
          end
          object N20: TMenuItem
            Caption = '-'
          end
          object N7: TMenuItem
            Action = ActionBlobViewTextWrap
            AutoCheck = True
          end
        end
      end
      object N27: TMenuItem
        Action = ActionOpenBaseNotMonopoly
        AutoCheck = True
      end
      object N31: TMenuItem
        Action = ActionRecoveryMode
        AutoCheck = True
      end
      object Bynary16GUID1: TMenuItem
        Action = ActionShowGUIDs
        AutoCheck = True
      end
      object ActionViewFileOffset1: TMenuItem
        Action = ActionViewFileOffset
        AutoCheck = True
      end
      object XMLBLOB1: TMenuItem
        Action = ActionXMLSaveBLOBToFile
        AutoCheck = True
      end
      object XMLBLOB2: TMenuItem
        Action = ActionXMLUnpackBLOB
        AutoCheck = True
      end
      object N28: TMenuItem
        Caption = '-'
      end
      object N1CDTool1CD1: TMenuItem
        Action = ActionAssociateFile
      end
    end
    object N29: TMenuItem
      Caption = '?'
      object N25: TMenuItem
        Action = ActionAbout
      end
      object ActionCommandLineKeys1: TMenuItem
        Action = ActionCommandLineKeys
      end
    end
  end
  object OpenDialog: TOpenDialog
    DefaultExt = '1CD'
    Filter = #1041#1072#1079#1072' 1'#1057' (*.1CD)|*.1CD|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Title = #1042#1099#1073#1077#1088#1080#1090#1077' '#1092#1072#1081#1083' '#1073#1072#1079#1099' 1'#1057
    Left = 200
    Top = 520
  end
  object SaveDialogCF: TSaveDialog
    DefaultExt = 'cf'
    Filter = #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103' 1'#1057'|*.cf|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' 1'#1057
    Left = 280
    Top = 520
  end
  object TimerGarbageMemory: TTimer
    Interval = 10000
    OnTimer = TimerGarbageMemoryTimer
    Left = 224
    Top = 584
  end
  object SaveDialogFile: TSaveDialog
    Options = [ofOverwritePrompt, ofEnableSizing, ofForceShowHidden]
    Title = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1092#1072#1081#1083#1072' '#1090#1072#1073#1083#1080#1094#1099
    Left = 360
    Top = 520
  end
  object SaveDialogXML: TSaveDialog
    DefaultExt = 'xml'
    Filter = 'XML-'#1092#1072#1081#1083#1099'|*.xml|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1092#1072#1081#1083#1072' '#1101#1082#1089#1087#1086#1088#1090#1072' '#1090#1072#1073#1083#1080#1094#1099
    Left = 440
    Top = 520
  end
  object ImageListEdits: TImageList
    Height = 12
    Width = 12
    Left = 784
    Top = 192
    Bitmap = {
      494C01010300090088010C000C00FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000300000000C00000001002000000000000009
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E4C1A0009F8F7F00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000E0E0E000BEBEBE00000000000000000000000000000000000000
      00000000000000000000000000000000000000000000F1DECE00C5BEB7000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FBF2E700C3834100A86B3100D1D0CF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000FBFBFB008C8C8C0065656500E9E9E9000000000000000000000000000000
      000000000000000000000000000000000000FEFCF900CA8D4E0096643500E9E9
      E900000000000000000000000000000000000000000000000000000000000000
      0000D5AE8100BC7E4100C280410092765E000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000C2C2C2007E7E7E007F7F7F008B8B8B000000000000000000000000000000
      000000000000000000000000000000000000E2C4A200BD804000C17D3D009A8B
      7E0000000000000000000000000000000000000000000000000000000000F2E1
      CE00CB965D00BD845000BF804000B67A4700B5B1AE0000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000F0F0
      F00093939300818181008181810074747400D5D5D50000000000000000000000
      0000000000000000000000000000F8F1E800CB975C00BB804900C0824300A770
      4200D6D5D5000000000000000000000000000000000000000000FEFEFB00DDB3
      7E00C18E6100C5936700BC7C3800CD94590095694900F2F2F200000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000B9B9
      B90091919100919191007B7B7B00929292007373730000000000000000000000
      0000000000000000000000000000E2BD9000C4905E00C28F6100BD7D3900CB91
      59008D6F58000000000000000000000000000000000000000000F1DBB800CFA3
      7300CAA28300C7986C00BD7F3D00CA8F4800CD9A6F009C8F8600000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000E3E3E300A3A3
      A3009F9F9F009A9A9A007C7C7C008D8D8D008E8E8E00B0B0B000000000000000
      00000000000000000000F7ECCE00D4A77100C69A7700C7986D00BD7D3B00CB92
      4F00BD895F00B5B0AC00000000000000000000000000FEF8F200E3BA8700CAA4
      8800D4B39800C6966900BF824200C8893F00DDAA6C00A2785A00DFDEDE000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FEFEFE00BEBEBE00A3A3
      A300B2B2B200989898008080800084848400A8A8A80074747400F5F5F5000000
      000000000000FFFEFD00E8C19300C99F7E00D2AF9400C6966A00BE814100C88A
      3F00DCAB7300926E5600F5F5F5000000000000000000F0D8AF00D7B69700DCC5
      B500D5B59B00C7976B00C0844500C78A4200D79E5100E1B68A00907C70000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000DDDDDD00B0B0B000BBBB
      BB00B7B7B70097979700828282008484840098989800A9A9A900979797000000
      000000000000F4E3C700D7B18900D4B7A300D5B59A00C6966900BF844400C78A
      4100D9A25900D3A57F00A1958F0000000000FBF3E500E4C29900C39D8800D6B9
      A500C79C7900C18C5B00C0844500C98D4600D79E5100F0C68400C19A7F00C6C4
      C300000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000FBFBFB00BCBCBC00B4B4B400D0D0
      D000ADADAD009595950083838300858585008F8F8F00B9B9B90084848400E4E4
      E400FEFCF800E7C49200CEAE9B00E1CCBF00CFAA8C00C5946700C1854600C88B
      4100D59A4900EEC48500A47C6500E4E4E400F2D2A200EACBA000D3A57200CB97
      5E00C48A4D00BA7A3900B6763800B97D4800BC865600C2916600CD9E7E00937A
      6C00000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000CFCFCF00CCCCCC009C9C9C009898
      9800898989007C7C7C007C7C7C008989890098989800ACACAC00C1C1C1007F7F
      7F00F3D8AC00E8CFB100C6997200C7966900C1895300BA7C3E00BA7C3E00C28A
      5000CC9A6400DAB17F00E0BDA3008E7B7200F9EBD500F3DDBC00F0D9B900ECD3
      B400E7CDAF00E2C7AA00DEC1A600D9BCA300D5B7A000D2B29D00D2AE9800D4CA
      C400000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000E7E7E700D6D6D600D4D4D400CFCF
      CF00CACACA00C5C5C500C1C1C100BDBDBD00BABABA00B6B6B600B4B4B400CBCB
      CB00F9EBD500F3DCBB00F0D9B800EBD2B300E7CCAE00E2C6A900DDC0A400D9BB
      A100D5B69E00D1B09B00D1AD9700D3C9C3000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      28000000300000000C0000000100010000000000600000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFF9F0000000F9FF9FF0F0000000
      F0FF0FF0F0000000F0FF0FE070000000E07E07C030000000E07E07C030000000
      C03C038010000000801801801000000080180100000000000000000000000000
      0000000000000000000000FFF000000000000000000000000000000000000000
      000000000000}
  end
  object OpenDialogAnyFile: TOpenDialog
    Filter = #1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Title = #1042#1099#1073#1077#1088#1080#1090#1077' '#1092#1072#1081#1083
    Left = 328
    Top = 584
  end
  object OpenDialogExt: TOpenDialog
    Filter = #1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofFileMustExist, ofEnableSizing]
    Title = #1042#1099#1073#1077#1088#1080#1090#1077' '#1092#1072#1081#1083
    Left = 496
    Top = 584
  end
  object EmptyPopupMenu: TPopupMenu
    Left = 784
    Top = 80
  end
  object SaveDialogCFE: TSaveDialog
    DefaultExt = 'cfe'
    Filter = #1056#1072#1089#1096#1080#1088#1077#1085#1080#1077' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' 1'#1057'|*.cfe|'#1042#1089#1077' '#1092#1072#1081#1083#1099'|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1088#1072#1089#1096#1080#1088#1077#1085#1080#1103' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' 1'#1057
    Left = 520
    Top = 520
  end
end
