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
      object AdvDirectoryEditStorageDirectory: TAdvDirectoryEdit
        Left = 183
        Top = 3
        Width = 515
        Height = 21
        EmptyText = #1050#1072#1090#1072#1083#1086#1075' '#1082#1086#1085#1090#1077#1081#1085#1077#1088#1072' '#1085#1077' '#1074#1099#1073#1088#1072#1085
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
        Visible = False
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
        BrowseDialogText = #1042#1099#1073#1077#1088#1080#1090#1077' '#1082#1072#1090#1072#1083#1086#1075' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
      end
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
        TabOrder = 1
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
          object VirtualStringTreeValue1C: TVirtualStringTree
            Left = 0
            Top = 0
            Width = 495
            Height = 322
            Margins.Left = 0
            Margins.Top = 0
            Margins.Right = 0
            Margins.Bottom = 0
            Align = alClient
            BevelInner = bvNone
            BevelOuter = bvNone
            ClipboardFormats.Strings = (
              'Plain text'
              'Unicode text')
            DragOperations = []
            EmptyListMessage = #1044#1072#1085#1085#1099#1077' '#1085#1077' '#1079#1072#1075#1088#1091#1078#1077#1085#1099
            Header.AutoSizeIndex = 0
            Header.Font.Charset = DEFAULT_CHARSET
            Header.Font.Color = clWindowText
            Header.Font.Height = -11
            Header.Font.Name = 'Tahoma'
            Header.Font.Style = []
            Header.Options = [hoColumnResize, hoDblClickResize, hoVisible, hoHeightDblClickResize]
            Images = sAlphaImageListConfig
            LineStyle = lsSolid
            TabOrder = 0
            TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes, toAutoChangeScale]
            TreeOptions.MiscOptions = [toFullRepaintOnResize, toInitOnSave, toToggleOnDblClick, toWheelPanning, toEditOnClick]
            TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect]
            OnFocusChanged = VirtualStringTreeValue1CFocusChanged
            OnGetText = VirtualStringTreeValue1CGetText
            OnGetImageIndex = VirtualStringTreeValue1CGetImageIndex
            OnInitChildren = VirtualStringTreeValue1CInitChildren
            OnInitNode = VirtualStringTreeValue1CInitNode
            ExplicitLeft = -48
            ExplicitTop = 16
            Columns = <
              item
                Options = [coAllowClick, coEnabled, coParentBidiMode, coParentColor, coResizable, coShowDropMark, coVisible, coAllowFocus, coEditable]
                Position = 0
                Width = 178
                WideText = #1048#1084#1103
              end
              item
                Options = [coAllowClick, coEnabled, coParentBidiMode, coParentColor, coResizable, coShowDropMark, coVisible, coAllowFocus, coEditable]
                Position = 1
                Width = 131
                WideText = #1047#1085#1072#1095#1077#1085#1080#1077
              end
              item
                Options = [coAllowClick, coEnabled, coParentBidiMode, coParentColor, coResizable, coShowDropMark, coVisible, coAllowFocus, coEditable]
                Position = 2
                Width = 152
                WideText = #1058#1080#1087
              end>
            WideDefaultText = '???'
          end
          object PageControlFileFormats: TPageControl
            Left = 499
            Top = 0
            Width = 289
            Height = 322
            ActivePage = TabSheetPicture
            Align = alRight
            TabOrder = 1
            object TabSheetText: TTabSheet
              Caption = #1058#1077#1082#1089#1090
              object Panel2: TPanel
                Left = 0
                Top = 0
                Width = 281
                Height = 294
                Align = alClient
                BevelOuter = bvNone
                TabOrder = 0
                object MemoValue: TAdvMemo
                  Left = 0
                  Top = 25
                  Width = 281
                  Height = 269
                  Cursor = crIBeam
                  ActiveLineSettings.ShowActiveLine = False
                  ActiveLineSettings.ShowActiveLineIndicator = False
                  Align = alClient
                  AutoCompletion.Font.Charset = DEFAULT_CHARSET
                  AutoCompletion.Font.Color = clWindowText
                  AutoCompletion.Font.Height = -11
                  AutoCompletion.Font.Name = 'Tahoma'
                  AutoCompletion.Font.Style = []
                  AutoCompletion.StartToken = '(.'
                  AutoCorrect.Active = True
                  AutoHintParameterPosition = hpBelowCode
                  BookmarkGlyph.Data = {
                    36050000424D3605000000000000360400002800000010000000100000000100
                    0800000000000001000000000000000000000001000000000000000000000000
                    80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
                    A6000020400000206000002080000020A0000020C0000020E000004000000040
                    20000040400000406000004080000040A0000040C0000040E000006000000060
                    20000060400000606000006080000060A0000060C0000060E000008000000080
                    20000080400000806000008080000080A0000080C0000080E00000A0000000A0
                    200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
                    200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
                    200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
                    20004000400040006000400080004000A0004000C0004000E000402000004020
                    20004020400040206000402080004020A0004020C0004020E000404000004040
                    20004040400040406000404080004040A0004040C0004040E000406000004060
                    20004060400040606000406080004060A0004060C0004060E000408000004080
                    20004080400040806000408080004080A0004080C0004080E00040A0000040A0
                    200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
                    200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
                    200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
                    20008000400080006000800080008000A0008000C0008000E000802000008020
                    20008020400080206000802080008020A0008020C0008020E000804000008040
                    20008040400080406000804080008040A0008040C0008040E000806000008060
                    20008060400080606000806080008060A0008060C0008060E000808000008080
                    20008080400080806000808080008080A0008080C0008080E00080A0000080A0
                    200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
                    200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
                    200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
                    2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
                    2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
                    2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
                    2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
                    2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
                    2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
                    2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
                    FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FDFD25252525
                    2525252525252525FDFDFD2E25FFFFFFFFFFFFFFFFFFFF25FDFDFD2525252525
                    2525252525252525FDFD9A9AB7B7B7B7B7B7B7B7B7B72525FDFDFD25B7B7B7B7
                    B7B7B7B7B7B72525FDFD9A9AB7B7B7B7B7B7B7B7B7B72525FDFDFD25BFB7BFBF
                    B7B7B7B7B7B72525FDFD9A9ABFBFBFB7BFBFB7B7B7B72525FDFDFD25BFBFBFBF
                    BFB7BFBFB7B72525FDFD9A9ABFBFBFB7BFBFBFB7BFB72525FDFDFD25BFBFBFBF
                    BFBFBFBFBFB72525FDFD9A9ABFBFBFBFBFB7BFBFB7B72525FDFDFD25BFBFBFBF
                    BFBFBFBFBFB72525FDFD9A9ABFBFBFBFBFBFBFBFBFB725FDFDFDFD2525252525
                    25252525252525FDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFD}
                  BorderStyle = bsSingle
                  ClipboardFormats = [cfText]
                  CodeFolding.Enabled = True
                  CodeFolding.LineColor = clGray
                  Ctl3D = False
                  DelErase = True
                  EnhancedHomeKey = False
                  Gutter.DigitCount = 4
                  Gutter.Font.Charset = DEFAULT_CHARSET
                  Gutter.Font.Color = clWindowText
                  Gutter.Font.Height = -13
                  Gutter.Font.Name = 'Courier New'
                  Gutter.Font.Style = []
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlack
                  Font.Height = -13
                  Font.Name = 'COURIER NEW'
                  Font.Style = []
                  HiddenCaret = False
                  Lines.Strings = (
                    '')
                  MarkerList.UseDefaultMarkerImageIndex = False
                  MarkerList.DefaultMarkerImageIndex = -1
                  MarkerList.ImageTransparentColor = 33554432
                  PrintOptions.MarginLeft = 0
                  PrintOptions.MarginRight = 0
                  PrintOptions.MarginTop = 0
                  PrintOptions.MarginBottom = 0
                  PrintOptions.PageNr = False
                  PrintOptions.PrintLineNumbers = False
                  ReadOnly = True
                  RightMarginColor = 14869218
                  ScrollHint = False
                  SelColor = clWhite
                  SelBkColor = clNavy
                  ShowRightMargin = True
                  SmartTabs = False
                  TabOrder = 0
                  TabSize = 4
                  TabStop = True
                  TrimTrailingSpaces = False
                  UILanguage.ScrollHint = 'Row'
                  UILanguage.Undo = 'Undo'
                  UILanguage.Redo = 'Redo'
                  UILanguage.Copy = 'Copy'
                  UILanguage.Cut = 'Cut'
                  UILanguage.Paste = 'Paste'
                  UILanguage.Delete = 'Delete'
                  UILanguage.SelectAll = 'Select All'
                  UrlStyle.TextColor = clBlue
                  UrlStyle.BkColor = clWhite
                  UrlStyle.Style = [fsUnderline]
                  UseStyler = True
                  Version = '3.0.1.1'
                  WordWrap = wwNone
                end
                object PanelViewIn1C: TPanel
                  Left = 0
                  Top = 0
                  Width = 281
                  Height = 25
                  Align = alTop
                  BevelOuter = bvNone
                  TabOrder = 1
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
                object AdvReflectionImagePicture: TAdvReflectionImage
                  Left = 0
                  Top = 0
                  Width = 105
                  Height = 97
                  AutoSize = True
                  ReflectionSize = 0
                  Version = '1.5.0.2'
                end
              end
            end
            object TabSheetHTML: TTabSheet
              Caption = 'HTML'
              ImageIndex = 2
              object WebBrowserHTML: TWebBrowser
                Left = 0
                Top = 0
                Width = 281
                Height = 294
                TabStop = False
                Align = alClient
                TabOrder = 0
                ExplicitHeight = 266
                ControlData = {
                  4C0000000B1D0000631E00000000000000000000000000000000000000000000
                  000000004C000000000000000000000001000000E0D057007335CF11AE690800
                  2B2E126203000000000000004C0000000114020000000000C000000000000046
                  8000000000000000000000000000000000000000000000000000000000000000
                  00000000000000000100000000000000000000000000000000000000}
              end
            end
          end
        end
        object TabSheetGeneratedTypes: TTabSheet
          Caption = #1058#1080#1087#1099' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
          ImageIndex = 1
          DesignSize = (
            788
            322)
          object AdvStringGridGenTypes: TAdvStringGrid
            Left = 0
            Top = 0
            Width = 789
            Height = 323
            Cursor = crDefault
            Margins.Left = 0
            Margins.Top = 0
            Margins.Right = 0
            Margins.Bottom = 0
            Anchors = [akLeft, akTop, akRight, akBottom]
            ColCount = 2
            Ctl3D = False
            DefaultColWidth = 250
            DefaultRowHeight = 18
            DrawingStyle = gdsClassic
            FixedCols = 0
            RowCount = 2
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
            ParentCtl3D = False
            ScrollBars = ssBoth
            TabOrder = 0
            HoverRowCells = [hcNormal, hcSelected]
            DragDropSettings.OleAcceptFiles = False
            DragDropSettings.OleAcceptText = False
            DragDropSettings.OleAcceptURLs = False
            ActiveCellFont.Charset = DEFAULT_CHARSET
            ActiveCellFont.Color = clWindowText
            ActiveCellFont.Height = -11
            ActiveCellFont.Name = 'Tahoma'
            ActiveCellFont.Style = [fsBold]
            ActiveCellColor = 15387318
            AutoThemeAdapt = True
            ControlLook.FixedGradientFrom = clWhite
            ControlLook.FixedGradientTo = clSilver
            ControlLook.FixedGradientHoverFrom = 13619409
            ControlLook.FixedGradientHoverTo = 12502728
            ControlLook.FixedGradientHoverMirrorFrom = 12502728
            ControlLook.FixedGradientHoverMirrorTo = 11254975
            ControlLook.FixedGradientDownFrom = 8816520
            ControlLook.FixedGradientDownTo = 7568510
            ControlLook.FixedGradientDownMirrorFrom = 7568510
            ControlLook.FixedGradientDownMirrorTo = 6452086
            ControlLook.FixedGradientDownBorder = 14007466
            ControlLook.DropDownHeader.Font.Charset = DEFAULT_CHARSET
            ControlLook.DropDownHeader.Font.Color = clWindowText
            ControlLook.DropDownHeader.Font.Height = -11
            ControlLook.DropDownHeader.Font.Name = 'Tahoma'
            ControlLook.DropDownHeader.Font.Style = []
            ControlLook.DropDownHeader.Visible = True
            ControlLook.DropDownHeader.Buttons = <>
            ControlLook.DropDownFooter.Font.Charset = DEFAULT_CHARSET
            ControlLook.DropDownFooter.Font.Color = clWindowText
            ControlLook.DropDownFooter.Font.Height = -11
            ControlLook.DropDownFooter.Font.Name = 'Tahoma'
            ControlLook.DropDownFooter.Font.Style = []
            ControlLook.DropDownFooter.Visible = True
            ControlLook.DropDownFooter.Buttons = <>
            EnableHTML = False
            EnhRowColMove = False
            Filter = <>
            FilterDropDown.Font.Charset = DEFAULT_CHARSET
            FilterDropDown.Font.Color = clWindowText
            FilterDropDown.Font.Height = -11
            FilterDropDown.Font.Name = 'Tahoma'
            FilterDropDown.Font.Style = []
            FilterDropDownClear = '(All)'
            FixedColWidth = 250
            FixedRowHeight = 18
            FixedFont.Charset = DEFAULT_CHARSET
            FixedFont.Color = clWindowText
            FixedFont.Height = -11
            FixedFont.Name = 'Tahoma'
            FixedFont.Style = [fsBold]
            Flat = True
            FloatFormat = '%.2f'
            Look = glTMS
            Navigation.AllowClipboardShortCuts = True
            PrintSettings.DateFormat = 'dd/mm/yyyy'
            PrintSettings.Font.Charset = DEFAULT_CHARSET
            PrintSettings.Font.Color = clWindowText
            PrintSettings.Font.Height = -11
            PrintSettings.Font.Name = 'Tahoma'
            PrintSettings.Font.Style = []
            PrintSettings.FixedFont.Charset = DEFAULT_CHARSET
            PrintSettings.FixedFont.Color = clWindowText
            PrintSettings.FixedFont.Height = -11
            PrintSettings.FixedFont.Name = 'Tahoma'
            PrintSettings.FixedFont.Style = []
            PrintSettings.HeaderFont.Charset = DEFAULT_CHARSET
            PrintSettings.HeaderFont.Color = clWindowText
            PrintSettings.HeaderFont.Height = -11
            PrintSettings.HeaderFont.Name = 'Tahoma'
            PrintSettings.HeaderFont.Style = []
            PrintSettings.FooterFont.Charset = DEFAULT_CHARSET
            PrintSettings.FooterFont.Color = clWindowText
            PrintSettings.FooterFont.Height = -11
            PrintSettings.FooterFont.Name = 'Tahoma'
            PrintSettings.FooterFont.Style = []
            PrintSettings.PageNumSep = '/'
            SearchFooter.ColorTo = 15790320
            SearchFooter.FindNextCaption = 'Find &next'
            SearchFooter.FindPrevCaption = 'Find &previous'
            SearchFooter.Font.Charset = DEFAULT_CHARSET
            SearchFooter.Font.Color = clWindowText
            SearchFooter.Font.Height = -11
            SearchFooter.Font.Name = 'Tahoma'
            SearchFooter.Font.Style = []
            SearchFooter.HighLightCaption = 'Highlight'
            SearchFooter.HintClose = 'Close'
            SearchFooter.HintFindNext = 'Find next occurrence'
            SearchFooter.HintFindPrev = 'Find previous occurrence'
            SearchFooter.HintHighlight = 'Highlight occurrences'
            SearchFooter.MatchCaseCaption = 'Match case'
            ShowDesignHelper = False
            SortSettings.HeaderColorTo = 16579058
            SortSettings.HeaderMirrorColor = 16380385
            SortSettings.HeaderMirrorColorTo = 16182488
            Version = '6.2.1.1'
          end
        end
        object TabSheetMeta: TTabSheet
          Caption = #1054#1073#1098#1077#1082#1090#1099' '#1084#1077#1090#1072#1076#1072#1085#1085#1099#1093
          ImageIndex = 2
          DesignSize = (
            788
            322)
          object AdvStringGridMeta: TAdvStringGrid
            Left = 0
            Top = 0
            Width = 788
            Height = 323
            Cursor = crDefault
            Margins.Left = 0
            Margins.Top = 0
            Margins.Right = 0
            Margins.Bottom = 0
            Anchors = [akLeft, akTop, akRight, akBottom]
            ColCount = 2
            Ctl3D = False
            DefaultColWidth = 250
            DefaultRowHeight = 18
            DrawingStyle = gdsClassic
            FixedCols = 0
            RowCount = 2
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
            ParentCtl3D = False
            ScrollBars = ssBoth
            TabOrder = 0
            HoverRowCells = [hcNormal, hcSelected]
            DragDropSettings.OleAcceptFiles = False
            DragDropSettings.OleAcceptText = False
            DragDropSettings.OleAcceptURLs = False
            ActiveCellFont.Charset = DEFAULT_CHARSET
            ActiveCellFont.Color = clWindowText
            ActiveCellFont.Height = -11
            ActiveCellFont.Name = 'Tahoma'
            ActiveCellFont.Style = [fsBold]
            ActiveCellColor = 15387318
            ControlLook.FixedGradientFrom = clWhite
            ControlLook.FixedGradientTo = clSilver
            ControlLook.FixedGradientHoverFrom = 13619409
            ControlLook.FixedGradientHoverTo = 12502728
            ControlLook.FixedGradientHoverMirrorFrom = 12502728
            ControlLook.FixedGradientHoverMirrorTo = 11254975
            ControlLook.FixedGradientDownFrom = 8816520
            ControlLook.FixedGradientDownTo = 7568510
            ControlLook.FixedGradientDownMirrorFrom = 7568510
            ControlLook.FixedGradientDownMirrorTo = 6452086
            ControlLook.FixedGradientDownBorder = 14007466
            ControlLook.DropDownHeader.Font.Charset = DEFAULT_CHARSET
            ControlLook.DropDownHeader.Font.Color = clWindowText
            ControlLook.DropDownHeader.Font.Height = -11
            ControlLook.DropDownHeader.Font.Name = 'Tahoma'
            ControlLook.DropDownHeader.Font.Style = []
            ControlLook.DropDownHeader.Visible = True
            ControlLook.DropDownHeader.Buttons = <>
            ControlLook.DropDownFooter.Font.Charset = DEFAULT_CHARSET
            ControlLook.DropDownFooter.Font.Color = clWindowText
            ControlLook.DropDownFooter.Font.Height = -11
            ControlLook.DropDownFooter.Font.Name = 'Tahoma'
            ControlLook.DropDownFooter.Font.Style = []
            ControlLook.DropDownFooter.Visible = True
            ControlLook.DropDownFooter.Buttons = <>
            EnableHTML = False
            Filter = <>
            FilterDropDown.Font.Charset = DEFAULT_CHARSET
            FilterDropDown.Font.Color = clWindowText
            FilterDropDown.Font.Height = -11
            FilterDropDown.Font.Name = 'Tahoma'
            FilterDropDown.Font.Style = []
            FilterDropDownClear = '(All)'
            FixedColWidth = 250
            FixedRowHeight = 18
            FixedFont.Charset = DEFAULT_CHARSET
            FixedFont.Color = clWindowText
            FixedFont.Height = -11
            FixedFont.Name = 'Tahoma'
            FixedFont.Style = [fsBold]
            Flat = True
            FloatFormat = '%.2f'
            Look = glTMS
            Navigation.AllowClipboardShortCuts = True
            PrintSettings.DateFormat = 'dd/mm/yyyy'
            PrintSettings.Font.Charset = DEFAULT_CHARSET
            PrintSettings.Font.Color = clWindowText
            PrintSettings.Font.Height = -11
            PrintSettings.Font.Name = 'Tahoma'
            PrintSettings.Font.Style = []
            PrintSettings.FixedFont.Charset = DEFAULT_CHARSET
            PrintSettings.FixedFont.Color = clWindowText
            PrintSettings.FixedFont.Height = -11
            PrintSettings.FixedFont.Name = 'Tahoma'
            PrintSettings.FixedFont.Style = []
            PrintSettings.HeaderFont.Charset = DEFAULT_CHARSET
            PrintSettings.HeaderFont.Color = clWindowText
            PrintSettings.HeaderFont.Height = -11
            PrintSettings.HeaderFont.Name = 'Tahoma'
            PrintSettings.HeaderFont.Style = []
            PrintSettings.FooterFont.Charset = DEFAULT_CHARSET
            PrintSettings.FooterFont.Color = clWindowText
            PrintSettings.FooterFont.Height = -11
            PrintSettings.FooterFont.Name = 'Tahoma'
            PrintSettings.FooterFont.Style = []
            PrintSettings.PageNumSep = '/'
            SearchFooter.ColorTo = 15790320
            SearchFooter.FindNextCaption = 'Find &next'
            SearchFooter.FindPrevCaption = 'Find &previous'
            SearchFooter.Font.Charset = DEFAULT_CHARSET
            SearchFooter.Font.Color = clWindowText
            SearchFooter.Font.Height = -11
            SearchFooter.Font.Name = 'Tahoma'
            SearchFooter.Font.Style = []
            SearchFooter.HighLightCaption = 'Highlight'
            SearchFooter.HintClose = 'Close'
            SearchFooter.HintFindNext = 'Find next occurrence'
            SearchFooter.HintFindPrev = 'Find previous occurrence'
            SearchFooter.HintHighlight = 'Highlight occurrences'
            SearchFooter.MatchCaseCaption = 'Match case'
            ShowDesignHelper = False
            SortSettings.HeaderColorTo = 16579058
            SortSettings.HeaderMirrorColor = 16380385
            SortSettings.HeaderMirrorColorTo = 16182488
            Version = '6.2.1.1'
          end
        end
        object TabSheetExport: TTabSheet
          Caption = #1042#1099#1075#1088#1091#1079#1082#1072
          ImageIndex = 3
          DesignSize = (
            788
            322)
          object AdvDirectoryEditExport: TAdvDirectoryEdit
            Left = 3
            Top = 3
            Width = 691
            Height = 21
            EmptyText = #1050#1072#1090#1072#1083#1086#1075' '#1074#1099#1075#1088#1091#1079#1082#1080' '#1085#1077' '#1074#1099#1073#1088#1072#1085
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
              FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F00000000FFF
              00000088888880FF00000B088888880F00000BB08888888000000BBB00000000
              00000BBBBBBB0B0F00000BBB00000B0F0000F000BBBBBB0F0000FF0BBBBBBB0F
              0000FF0BBB00000F0000FFF000FFFFFF0000}
            BrowseDialogText = 'Select Directory'
          end
          object ButtonExport: TButton
            Left = 700
            Top = 3
            Width = 85
            Height = 21
            Anchors = [akTop, akRight]
            Caption = #1042#1099#1075#1088#1091#1079#1080#1090#1100
            TabOrder = 1
            OnClick = ButtonExportClick
          end
        end
      end
      object AdvFileNameEditConfigFile: TAdvFileNameEdit
        Left = 183
        Top = 3
        Width = 515
        Height = 21
        EmptyText = #1060#1072#1081#1083' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' '#1085#1077' '#1074#1099#1073#1088#1072#1085
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
        TabOrder = 3
        Text = ''
        Visible = False
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
        DefaultExt = 'cf'
        FilterIndex = 0
        DialogOptions = [ofFileMustExist, ofEnableSizing]
        DialogTitle = #1042#1099#1073#1077#1088#1080#1090#1077' '#1092#1072#1081#1083' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
        DialogKind = fdOpen
      end
      object AdvComboBoxVariant: TAdvComboBox
        Left = 2
        Top = 3
        Width = 175
        Height = 21
        Hint = #1042#1099#1073#1077#1088#1080#1090#1077' '#1074#1072#1088#1080#1072#1085#1090' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080' '#1076#1083#1103' '#1088#1072#1079#1073#1086#1088#1072
        Color = clWindow
        Version = '1.4.1.0'
        Visible = True
        Style = csDropDownList
        DropWidth = 0
        Enabled = True
        ItemIndex = -1
        Items.Strings = (
          #1060#1072#1081#1083' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
          #1050#1072#1090#1072#1083#1086#1075' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1080
          #1054#1089#1085#1086#1074#1085#1072#1103' '#1082#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103
          #1050#1086#1085#1092#1080#1075#1091#1088#1072#1094#1080#1103' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093)
        LabelPosition = lpTopLeft
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        TabOrder = 4
        OnChange = AdvComboBoxVariantChange
      end
      object AdvFileNameEditBaseFile: TAdvFileNameEdit
        Left = 183
        Top = 3
        Width = 515
        Height = 21
        EmptyText = #1060#1072#1081#1083' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093' '#1085#1077' '#1074#1099#1073#1088#1072#1085
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
        TabOrder = 5
        Text = ''
        Visible = False
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
        DefaultExt = '1cd'
        FilterIndex = 0
        DialogOptions = [ofFileMustExist, ofEnableSizing]
        DialogTitle = #1042#1099#1073#1077#1088#1080#1090#1077' '#1092#1072#1081#1083' '#1073#1072#1079#1099' '#1076#1072#1085#1085#1099#1093
        DialogKind = fdOpen
      end
      object PanelStatusBar: TPanel
        Left = 0
        Top = 360
        Width = 793
        Height = 22
        Align = alBottom
        BevelOuter = bvLowered
        TabOrder = 6
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
      DesignSize = (
        793
        382)
      object CheckBoxLogToFile: TCheckBox
        Left = 3
        Top = 2
        Width = 86
        Height = 17
        Caption = #1051#1086#1075' '#1074' '#1092#1072#1081#1083
        TabOrder = 0
        OnClick = CheckBoxLogToFileClick
      end
      object AdvFileNameEditLogFile: TAdvFileNameEdit
        Left = 90
        Top = 2
        Width = 703
        Height = 21
        EmptyText = #1060#1072#1081#1083' '#1083#1086#1075#1072' '#1085#1077' '#1074#1099#1073#1088#1072#1085
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
        TabOrder = 1
        Text = 'c:\MetaReadLog.txt'
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
        DialogOptions = []
        DialogKind = fdOpen
      end
      object CheckBoxUseExternal: TCheckBox
        Left = 3
        Top = 25
        Width = 222
        Height = 17
        Caption = #1054#1090#1083#1086#1078#1077#1085#1085#1086#1077' '#1095#1090#1077#1085#1080#1077' '#1074#1085#1077#1096#1085#1080#1093' '#1092#1072#1081#1083#1086#1074
        Checked = True
        State = cbChecked
        TabOrder = 2
        OnClick = CheckBoxLogToFileClick
      end
      object AdvEditExportThreadCount: TAdvEdit
        Left = 163
        Top = 45
        Width = 86
        Height = 21
        EditType = etNumeric
        EmptyText = #1055#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
        EmptyTextStyle = []
        LabelCaption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1087#1086#1090#1086#1082#1086#1074' '#1101#1082#1089#1087#1086#1088#1090#1072
        LabelPosition = lpLeftCenter
        LabelFont.Charset = DEFAULT_CHARSET
        LabelFont.Color = clWindowText
        LabelFont.Height = -11
        LabelFont.Name = 'Tahoma'
        LabelFont.Style = []
        Lookup.Separator = ';'
        AutoSize = False
        Color = clWindow
        TabOrder = 3
        Text = '48'
        Visible = True
        Version = '2.9.4.2'
      end
    end
  end
  object sAlphaImageListConfig: TsAlphaImageList
    DrawingStyle = dsTransparent
    Items = <
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_58'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000001F4944415478DA
          62FCFFFF3F032580898142306AC0A801A3060C160300020C00639A031D5B62E3
          C60000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002A84944415478DA
          A4534B4F1351143E77E6B665FAA6EDD0F2289556103168D4C407A282616348FC
          070671E5C6F8075CBA2451774617C6A80B17BA30CAC28836B12444C504A20B4C
          071443806999B10C6DE77DBD436341D18D9EC9993BF9CE9CEF7CF7DC7B102104
          FEC7F0EFC0C2F4554C6C3B987D39955EC94B01846D3DD111D507878E08D8E52A
          B51DB8666DFF1F6D57F061FC52B8BA21F713CB3B1A6ADE7F063CE1206208908A
          B82E7D7B97E538E37673269D4B1EBAA1EC2078FF74249C9F13477BFB4E5E696A
          EF6D0F44D2C8E589D3A089D4B200A5E227B22C4CCD835E188BF1AD0F53476F29
          7582B9EC455C281487837CE7CDD4DE8194AFB10B18D647C3B43CA20FB1C1328A
          505A9D84F9D9714155362E277775BDE8E8BB63310E8B65DA414D952E44E3FE76
          CEE7062032D8E66ACD0D675D0184AAE00FF3D0D4C6A7CBA5A5F3B66906EB4DCC
          4DCEEEDED78D071AF02A22C62225E42809A211677B7445CEB705C85E07CE5D42
          3EAF7DE67576269D3905D39B044B5FC5484FA73B64573E822E55681E0B8EF63A
          81F3DE24D101D42FE0622B3171C1F6D51510A26B96AA115DFA0C505E03C48428
          CAFE4C2635220B8825815175B647BBC3585A9DA03599D8B0345936744F141357
          2DA756784B026D2480872687C154B5423419311D78B3897DC7BAF31515BFAA6A
          09429814455B281AFFD5995670629A1127A6EE9E38DDDF236CDD44622A6E2E76
          6F4DC287B9603CEDF736D2F3C5DBCA3B524CA856D7882C2BF9061FFF0081A5D4
          15F40E3F31BDDE484E9853AE2F2FAF0B8AC612E0787085BA0153475C0C2A3A43
          C41525CF62768C6F8B4F760E3DB2765CE53777874286AE9D201018F147338336
          E38FB02C83902117D5EFF3130C2ADF8F2542B93DC3CF943FCE8263338FCF620A
          05DECE8A1971510F307498F8168F76FC605C605846E939F7DCFCEB30FD8BFD10
          60009D813D909670E9420000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_02'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002E74944415478DA
          9C525B485451145D77C6E748281396888CE5235FA1E59822BEC8C81C25313386
          2CC94A33A2C01F530882A28F0A0243B324A81F1384FA2891FE32F09531532134
          E898E644D334A3E3307AEF9DB9F3B8B77BCEA4E04F1F1DD8AC73F6D96BEF7DD6
          3E8C2449B86C1A884FE1DB2FCE8B837566DCCE678DD59F9C265DAF5BEC7FED7E
          34C2E31F2BAC72F2D8AED2A4EAB1A828365BE36AC4AAD30C21B8A33852251447
          29D786BECC7EB67A79A1D366B3C1E7F3213636166EB7BB59AFD70F92044CD787
          BEE726CD70CB01E11E247B1A7EDBFC5858B2C0AEB80F36308E9E8A01E8727488
          888800C330840C83C10097CB552027312A62A4AA16DE0A18F93BF8EE9EC606BB
          0EC1CF22A0B440F48B18997B05954A45C94AA5126AB51A5AAD160F96620CF409
          73CB01C4C4E9C086CFC2EF8AC78A6B05EB1B1C3C41095E671EBE790A4174224B
          1445BA27CFD8D280E1D7C03B8E20A84C8749DD042F7B14EBCE72F0D65A300A0E
          C9495994140804282A148AED22EEDF1D8D19B3190CC2E58A39F0FA27C1737990
          023BE5EB7874D7E752B22008B403928018C7858613569C9680859F2B189E5C84
          223213A2A095DFEE93AB8BB8515F80D48438B02C4B4924413018DCD6459861C9
          8E87CDE5A8C9D560D4B81766DB1A3212E3D059770889B1D1E0643211908C9020
          31BFDF0F8EF784329CEA7D2BF13C4FCDE3F15093DBA5288F8C22C77192DD6E97
          BC5E2F3D137C31352F114DC2EAB52978373183A7432F651D18B49D3989C32545
          3858D5480B64A7A7A2B5A901A585F9189BFA888E9B7791959E82BC924AA0781F
          30316F954E9CBF46AB912A75E7AE480E8743CA28ADA1487CB567DBB7EE36BBAA
          D05F0A7540AA981616E15CE7E898E61797F1D51E52B8ACA105297B34C82D2A87
          C9E1A17734CEB78A0B573B68CC969CBAD3ADA8696AA37B46C1507CF2B81FD7BB
          BA313DFE9EFA929335F8C54BB005A2F0ACAF27144BAA6696D592AFC6FCCDB5B9
          A7DF8F902C961F7573E3A32372DC71D9F54636A36CB7888FD9FCA6FFBBFE0830
          00FE069F3DDB3ED2370000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_04'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000C54944415478DA
          62FCFFFF3F030C6CEB4C6D025225EFDEBDE3F8FCF533E3A70F9F19BEFFF8D9DC
          B0E65C1D030EC082C6AF750E4D62F8F8EE35C3C7F76F195E3F7FCAB073E39A5A
          A038D1062CD9B56276CCDD7B0F18BE7FFBCAF0E3E72F863F7FFE3633E003202F
          C0B07D62852F109F81B2CF80F8C8F2D83056CD4862040DC1A999584318B776A4
          E00C796262858540C8138C151602214F3856F0051031B14294667C014AB4665C
          8630C2F202A110C725CF42423EC02ACF42423EC02ACF889C9DC9010001060079
          B3E43D049D59B30000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_06'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002724944415478DA
          A4535F48536114FFDDEDEEEAEEE696DBF2EA2C37E7121132E992D01F0C45D99B
          42E94BD45EA528A4879ECA20059F7AA888222A7C9040310C83A8207AF145925B
          A6A5B296D59A637537D7CC79756CF7F6EDEA4AB8D64B077EDFB99C3FBFF39DF3
          9D4B75DF18B904E000B6484D05575C6AB3982EF7F6D5A4D724165A1108E6E7C7
          9F9CA473C9D7CE75B46565591335EC75A3B5F110D6D3695CBF37087F673B0C0C
          B372FFC1C8AD7C515D3E5896150D0A0A18F89A8EA0CDD7ACC61C6ED88F89990F
          EFC8E72CC1A79C8DC63FE4EED53E55BF9E7EAFEA22B329EF9AD8C41F0203ADFF
          2B516A5542C52E27AA2B5D1A9F4AA028327E466F23161845EAFB4635DAC26387
          A7135CD5297C1363A87096C1C41AB5041439224357401BFAE1A9CF00478164D2
          0A510CE0EBC74790520C2262065CC94E90B1684477F0E94825EDEF07B7B48F64
          124B1088C7BDD0E98EA3C13706143722105E40216B42769BF6E8D2B9374E2E43
          2A7791A7BDC303BC800471248A3DF831338B13672E6C864E626D35A525606B5B
          2CD9EA16E8632132F68D410A3C8F22A25BF6D6822C0BE22B4B584EB3283133F0
          9FEF516DBF0924724CB9EB912420B707150CE0799984A9C40B9C9E1C26F348A2
          3ECC60B0EB19B2ACF6D5E9B0B9689903ECB964128B61542339EEC58237B8B9B1
          C01E238B34D946395BA825089697471291A83D142F4522D7BC13B092BBF0024F
          5A01B86814CD4DADA82A77C0B8CDAAD021876560E1D5E76ED3325C02B308FD62
          1D99B6012813D0F4D286A46DFECBD0C0E3B7046A82DDA80B6C25A07A1D0E8CB9
          8E51F6F5D8596346EAD029BBEBD47FC3434F4B46EBC3B8C37DB37DF4A2D2238A
          DB6E29A5280AFE477E0930005349E5C6B629D2020000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_08'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001414944415478DA
          62FCFFFF3F03258089814230F006B0A00B348B8A3284D57D0AFEC3C218F79B9F
          D1EAFB374E866FBF798FFDFCC3BDE86CD3EBB5B5AF5FE337C0E7CB17A73F3795
          3AC49D6FABF0A9FC6378C0A4C5F0EBD61FBFABF765B404EA94414AD6E2F502DB
          FFFFF9E2336EAB88BC366060FBF59FE1F30F46869B72160CAF14F955F6FCFE17
          47D00BBF759CADF8FEFD636098CD06E69F3536667808A4F78A7232885C7C6E45
          D08087C2320CAF657419EEB3B03130DEB9C5B053F23BC305CED70C9F9EBD66F8
          FDFC39E140FCC9C27AECE9F78F7E5FFF8932AC645063F8785885E19ECA1D06D1
          D707187EB3721D2368C04B2696452F6F3DD7FAF09B418545869F41F8CF350699
          27D20C6F8584EEFC61115B84AE9E113D2983A2F1AD6874F0FDFFB7E398FFCB58
          31083030FC15663DF68F497D91D989268C68641CCD0B0C0001060005F87B65BD
          0499C20000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_10'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000021C4944415478DA
          A493CF6BD44014C7BF33C924936CB6DD2D98C5EDA15D104516E9C51F28FE805E
          FC038A5EBD29FE05D293773D7AF12C9E0A2D14040F8516C44BA1420515FCC1D2
          8358EAB6874D6D926D3233BEA4166A8DA7CEF0986466DE67DEF7BD19668CC149
          9A7DE47B8AEC611445D7CB05DBAE74F07D7F9E8605B2F7C701F756D73E3C58FB
          BC0930863CD730D441112AA5C1A0B19F65B87CBEDD9DBE7965E19F080683C1CC
          EB771BC8E46914B206510261919B2680D607A614E2F5EFE89E9B7CDE6AB5AEFE
          05E09C5B81E742D5BC1260718ED9BB174A08B738A25F09E656BEC2C4AA3A0744
          E7637581D150224A0CFAF91E5E2CBEC1ED6B5D18C6B1FA651B42D041AE450A69
          E238A00C5B33EC0D19149D6E461B485281E58F7D8CD42476730B9EEB201E962E
          AC0200342447BD713065310F5C09B82C872738C602826B07D2C86A09B66DE926
          4BD1E2BBA5FE9A149417D2CF05FD330ADB14A1E3C7BE2EB6EBAA7B80C5B74F90
          9A9FE8DAE388F5297C8A37A0C526849498503790C731C17CCC3E7A6C2A01BEC8
          314876B06E76E0D04A7D8472A18BD363F4ED576001D00E2E564B289228CF2874
          320E46DA139D21A08AB80E476C9565A64D0CE976540DD05AABCC4AC143876054
          896F94B42D83480FE1B55DBA93C5CD54F095530D68369BCF6E4DDE7FBABCF512
          2E246AE37F8A456A79D1A9F405E0D2D9198461387FE8C78EBCC63B6413BD5E6F
          9A1529FF4FEB743A2B342C1D3E2676D2E7FC5B80010037F5D135634FB1A40000
          000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_12'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001114944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FF27110465
          55ED05E9C508838D3BF731F4CF5A046683686436480E6B1820837B0F9F3014A6
          C581D9301AC6861986D580BFFFFE33303331C2353D68AB66F874FA1898CD676A
          C5A050D50A9703A9450406340C7EFCFA03F65BDFCC8538FD0D93FB0E540B0B03
          B80B7EFDFDC7C0CECA0C37189B0B60E037502D860BDE7CFE4E740CBCF9FC0333
          167EFDF947742CFCFAF31733107FFE8618E0EFEE8412F2D8D8580DF8FDF72FC3
          EDE71F211C4698FF90E28B9111E45F547998018F1F3DBC965AD5318394A4CCC9
          C2F8086C16A5D91920C0004F75146EEE3E659A0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_14'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001714944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FFFBF7EFFF
          DFBF7F89C220B54159557B417A612E60F8F38F81819911D3A6BD7BF7A2F04545
          451974F5F451C300047E0323E3C5B3E70C5FBF7D01F39594941940F1232B2707
          550132FD3F0317170FC35FF44004815FBFFE32FCFAFB8FE1EFBFFF608DB76EDF
          018B3F7CF490810129A68545841944C525310D00015E4161065E211150C0406C
          640479ED1FDC05D2B20A108760730148D9E54B97193EBC7F8B110EBF240DC08E
          B876EB3D8330172B83AD2A3BA6012005322AEA0C3260BB1819206EF8CF006580
          1DC50855F71F6B18FCFEC3C0727C0F03CBAB2728B67FF28A4509C4FF40FA2FDC
          5B4806FCFDF397E1BF9E2803E37FD4D4FDFBEF7F243742E87F7FB118202CC0CD
          F09BD79E81092D9084601620B99B9319CD0B8F1F3DBCE6995A398394A4CCC9C2
          F808EC314AB333408001001A78AA19CE47B0300000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_16'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000F94944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FFFBF7EFFF
          DFBF7F89C220B54159557B417A612E60F8F38F81C1AB6B3B8A2DADCEA260BA7A
          EF6B1431631353D4300081DFFF111AD001BAF85FF44004815FBFFE62687CFAF4
          295603750D4C300DC0061E3F7E4C5C3482C03F34BF824096862CF618C066C07F
          2C7E3D71E204F106FCFAFD0743A1AC2C7617FCFDF70FD380BF7FFE121D06FFFE
          62314058809B81CFC08C818911A150CBD00C612B92BB3999D1BCF0F8D1C36B9E
          A995334849CA9C2C8C8F403423A5D91920C00033DA834710785C6B0000000049
          454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_18'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001394944415478DA
          62FCFFFF3F032580055DE0CE89C21A20D58C4DF1831DF78A5D1A36F6E13500A4
          59585C0844AF04E2A550B168200EFFA4F4B11748A318C084AEFBDB4F2686B72F
          DFC1B89BA11828F696E1F387CF84BD00024F1F3D65F8FE9323FCCBC797E1CCCC
          AC0C9C3CC20CEFEE3C6578F7FA1D710680C0FBCB1718185E7D66F80D64FF10E3
          6360E0E4C6AA0EC30B3F5E31E4FC7EC7CEF0E5232FC3D3A76C60FCE5230FC3EF
          DF1C0CD27A0639E8EA197145E3ECD9B39F9C3871425A404080E1F9F3E70CCB96
          2D6324CA0530A0ADADBD43515191C1DADA9A0164082E80D380BB77EF8A000198
          4D960106060667989898CE82D8CCCCCCA41BF0EEDD3BAF7FFFFE19333232E24D
          CA380D1014149C0863030D22DD806FDFBEFDF9F1E3C72942990DA7011616166B
          8186AC047A6533302C701AC048697606083000E943747569569D940000000049
          454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_20'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000015F4944415478DA
          62FCFFFF3F03258089814240B1012C30C6A9CB0FFF7FFEF693E1DDC76F0C306F
          C9490A327CFEFA83E1ED87AF0CFF90BC7AF7FE0386DAEC10463007A41884771E
          BDFE1F1D6C3D74F93F3650DBBBF43F4C1FDC05AFDE7D6258BCE924C3FF7FFF19
          FEFEFB03B49181E1DFBF7F0C33571E00D90266C3C0EFDF7F107E80993467CD21
          0C9B262DDE89D50585CDB3305DF0F3E72F30BD73EF690651110106237D5586EF
          DF7FA088810048FC17542D4A20B2B3B38169776753B824172727861808B0B1B2
          621AF0F5EB370C177CFDFE1DAB0BBE415D8662C0972F5F19FE0003CED90962DB
          EFFF98622000E27FFFF11DD3804F9F3F33FC0406F4910310DBD47554193E7F42
          15030190F8B76F585C70FDE64D8657C0B051B306DA06B4E5F56FA0D8AD5B6031
          752B5306583202F19F3F7F0137801196EA5CC30BFF7FFBF1031C35FFFEFE63F8
          07D4C2CEC6068C9D9F5893F0A96D7318510C18B0CC0410600056621588318C02
          080000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_22'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000934944415478DA
          62FCFFFF3F032580898142306A0003030B32A77AD6C62620558B4DE1DF7F7F18
          FEFDFDCFF0E1E3C7E65955C975580DF8F5F3476D7AB80F50F15F863F7FFF31FC
          FD0BA221EC3F50F69445EB4016E032E027C391CB77197EFDFACDF0FBF71F869F
          BF7F43D87F806C20FDEBCF6F06361616DC5EF8F9F3E7929DBBF7C4E0F5330B4B
          33329F7134250E020300020C00EA6550DA516B629D0000000049454E44AE4260
          82}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_24'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000016F4944415478DA
          62FCFFFF3F032580898142C082CC61646464B0C9EE8A66E7E29D0EE2FFFCF639
          F3C8D4B2A5F85C89620048B38CB6E912232D5530FFDCB5DB4B806220E652A2BC
          00B2594E4181E1C38F3F600C62C35C43940B40E0D38FBFE40722C8CF1F5E3C66
          E0E3600663101B24060A1B5C80113980B004A23530102F77C7DB44FFF8F1631E
          488C838323A974E11178C032124A073D09B6D1D252524B0C2CEDC0FC0BC70F31
          3C7DF62CA664C1E1A5F03000DBDC29160D642D64E164FCF6E7FBBFCC23E5AFC0
          B6FCFEFD7BA18C8222C39BE78FC10680D877EFDD9B078B19B00120CD826A6C4B
          024CC2405CDE0D67562D018A81A3EFEFDFBF0C9FDFBFC51F882C9C4CD3D595D4
          18AEBFBB00C62036480CEAE7F88F1F3F31F00A0A8331880D0A079440749C2CF9
          49CC888317D9E457E77E7CDE9FFB9C0FE43D7C8108F602C8CFDF5FFF5962A7E3
          08163C74653F580CC4862A5C8A9C1A4B16A045232C1061CE066AB60606E26562
          722A23A5D91920C00095DBBD1D0D719C090000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_25'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002534944415478DA
          A4534D68134114FE368D9212DB98B4C52C1A7FB08DDE44CCA911EA454590D242
          4E5614450B5EA427B12DA5B17828A247A5E0C534167BF160D51CBC7B08E2A189
          153507358D0DA49A3431C92633BB3BCE4C4C6C6A2EE2076F67783FDFFBDEDB5D
          E5E6F8042BE4F370381CD8E5565187D3E96ADC0BF90D7966321994CA6579BF77
          F78E224EABD77B08FDFD7EB85C2EF4743B65305F2820B79107A514954A05BAAE
          A3CC0B4BC512EC763BC2E1F906B9B2964EB385C70B3872F4183C9EDD38ECED93
          81E5F83B689A866CF607D633DF91CBE5E0F3F9109E0F21189C66AAAA5A449E45
          75BB31727E0489C407AC2653B2B380E8243A5BDBB6C166B361E0C400EF1CC254
          70125DDD3DACAEC0F2F0F9EBA78264786808DFD65288F1CE02A28854094CD344
          6F5F2F169F2CF2CE41EC513DB87DFFD1DB3A8175E54B7A5D371904C9E95327B1
          B4F442EEE3C0FEBD5C7E16841044A3518C8D5D973926CF5D497CFED950201EC4
          04A86ECA84C1C1B38844227CFE2AD2E9349663318C8E5E9131424DE868468D80
          1828861EA0303B2913B5B226B74F880ECA4DF80AB313D096DFC0D0591381B57E
          A181CBA08A824E41484923418C2050BD36C5DF19B0BD95023E012A7C8E4AD5A8
          91D13F42E96F32F3C625683CCEB6104805C2A9E99B8AC86605B4F6714DCF8171
          62D672073C40A821972410080CE3D6CC0CE2F1182E5E38277D894F1FF1EAE533
          18A6F9B7024337400D26B5A4B2251CF7FBA5D5217CEABE83D24CA30541D74E3B
          3A3BECB02835679136CBECD8619726D0DEB66507ABC9AFEFCF5C1D9FC33FA0DD
          AA241B3F13630CFF835F020C00EDC0156999C1CD4E0000000049454E44AE4260
          82}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_27'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001284944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FFFBF7EFFF
          DFBF7F89C220B54159557B417A612E60F8F38F81E1D78FEF0CDFBE7D2368BB90
          B0086A1880C06F60647CF9FA8D61E58AE55835B9BABA32686A6A82D9BFFEFDC7
          34E0D7AFBF0C3CDC5C0C919191386D7EFDFA3598E6E117C63400048875C1F75F
          FF300DF807C52085B8C0F5EBD7C1B4BCB23AA601205F01039861F7EEDD70C9BC
          BC3C14CD30177CC5E6825FBFFF60B80066233A5F5A5115D380BF7FFE025DF00F
          AF0B4000E48A0F5F7F611A202CC0CDF09B979B212913A1E9CB6F84ED52CA10E7
          7F04EAE5E2604335E0F1A387D73C532B679092943959181F8168464AB3334080
          0100F279A82D29BB34FD0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_29'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001874944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FFFBF7EFFF
          DFBF7F89C220B54159557B417A612E60F8F38F8161F6F6D30CE6EA320C7DEB8F
          E3B4D95A4B9621DDCB0C350C40E037303252DC4D18FE03E1FCC20088EB809811
          E664300B28F29F91E12F7A2082C0AF5F7F197E2E9FC1F0FFEC31B816E6C40286
          BFF327A018C39C54C0C0A6678A6900D81521490C0CA1C9A080016BFA03D2D73E
          172ACB0836E62F9060C3E602601030FCF8F50FCC66AF4981181893CBD0BF7913
          43A1AF2F03EB9229602FFD6C99C3C0CD81C50090E4F73F7FC0EC1F0D33A1FEFF
          CF905EA0CDF00D68EBFFFA199050F8FD87E13FD630004AFCFAFD17EADFFF0C73
          2676C215B9F90633ECDABC164CDFBB7D83213E361AD380BF7FFE32FCFEFB1FEA
          160686F89C3294E883F125E59519FEFDFD876980B00037031F2F37031323F6F8
          FF8BE46E4E66342F3C7EF4F09A676AE50C529232270BE32370DC509A9D01020C
          007F09B7A808136E8E0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_31'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001ED4944415478DA
          62FCFFFF3F033270CAED329595950DE2E7E1B27AF7F1B3F2EBB71FEE7EFCF869
          F7E74F1F765E5DD9759A010DB0A00BF00808F56504B9D8F00A8932B032FE6778
          FFE5BBF4D96B77ED44597FA403A565310C302B91F8FFE3F57F86DF1FFF335CDF
          F08A11086CEE7E6162D01766603872EB05C3C92B77181E3F7DC9D018622C639A
          2F71FDEFAFFFD7BF3FFFBF9E578569D1A99E178C4C9FEFFC8B636462B8C1CACF
          C840089C9EF84213A8792D9B206315C8521060E294640CFCF5FE7F0B8728C200
          460658B8FC87093024B52D003381362FF9FF8F4103E462B01798D91835AF6F78
          B914C85E8A6E23230323B23160007236B21A2690B3900580B172E4ED9B372806
          FDF9F10D2473049BB73062E1CB87774567CF9D0FBA73EB263C1ABF00A3F1FFEF
          1F3BB119C0E898D34930DEF1A50D165CF13E7DF16A77A05E5B7C6903A486053D
          DE6DE4B8186CD52518CCAA936CE0CEC491368032362CD8FC155EA0CD004A17C0
          A8FD0F0AF58638770671713E065E0E602CA0A965C216EF20CD92A20A0CC04416
          03E2DF7D7499A173793A46DA801B801EEFA044E56914CF004C64C1A0E4BBE7EA
          12065F9314AC6983051AEF360CF222708366675F60B875FFC99373D35F0681F8
          3B4EDF78AC2A2F2BF3E93766DA60418FF795485114EA640256DCB56853107A34
          C2D206408001001A9D2CD823A3B1140000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_33'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002784944415478DA
          84535B489361187EBE7FFFFF2B3B7898CEAC5C660DD20A22634B651985781505
          233008EAA6483AA05817110405DE249DBCC82E92A0835074F2C6488DCAA190CA
          D20B6D9A2E2F329ACEB94D171BFE87AF7FFF70CD69F4C0C7F71EBEF7C0FB7C2F
          A1942219072F3459CD66B32353AFAD980F2D6EF5F9839E5068A17B7121D839FA
          BC69102960530DFA2CE3ED5A4795DD6034812314817064A3EBABA7D2C445CF28
          6EF3AA04B64BF934EAA3104214EEF659A2C0EE0933D89503F47EF3A27F64123F
          7ECEE0FAD13D05D6BA7CB7B444DD915FF48DC1C23C1EB8E925CCE2A47C823018
          E33209FE87C1666F8912FC8ACF2657624555ECAECD7B5D7CC474DC7A711D8DCD
          E3F0D556FA74C84753F1D135A6FA9DA39DAAFE61E09DAAB31A9E94B8DB67DA94
          5C6DA9157B8627C0B21A55E6780EEF5DE3DF81C229E50698CD38D9706D818DB5
          951CA464EDF5CFCDD9815C4C4CFB70EA500582A505AACF7DEF4551ECB6D9F6AA
          FAFD87C2E7552C8483F30DAE2F430E54155F5EB68DB5BC44595919CAD7980B9B
          CA3B97A6F58C4F4C752F3F10240A49A68808F28A405E43A098F5ECBF784F2450
          86EDECF9147B0CABB125CEC6FC596818C5279342369577FB262DF66DCB4F5412
          0509E5F6FDB0EC2C4500ADAACDB201C8D672901F751898B5F8AEA9DF81E40E64
          85AEA8B4F24834E92B132CEF43FC4EFE544B8288FE3E2742FE5944BB1A555B4E
          CD5D4C73ECEA5D208807A69BFE26904409D5C74EABB22FF30EF28AB6ABF296BC
          0CE04907D804EF85B989A007E78613724E960E19061D182567D7B33ED8D37954
          8E70AAAFB9D4229203E76FACB9BE6F6FD5553BEA1B03F1D648169F96BE9EE738
          A328C9BA2541F82D8982CF9806F18F0003004C6839686216DE56000000004945
          4E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_36'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000D14944415478DA
          62FCFFFF3F032580099F64C72506DFCCF5777DC93200A419486D02617C863011
          D00C03380D61C2A7F9F1E5930C5FBF7CC16B08133ECDB78EEE64F8F4E1035E43
          98F06986B9009F214CF834C3003E435840C4DEAD5BE18AB9797818242DFD1954
          5CA3C17C7965650684C11B510C6608DC0A71C1EE4AEFCD40CA0F876DD85CE577
          E3F056901E0646E494E8DABE15EE15904BF804041878997EE0D48C61003643BE
          5E3F885333D67480EE1D7C9A71A6446443F06906039017706197B62DBEEA365E
          BEF8D430529A9D01020C007CA4D6229E8CA9670000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_38'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002354944415478DA
          A453CF6B534110FEF2CBB4A92535358134D893604104A95E8B68CF89C782148B
          FF812078AA788822E8C1E2C11EBC580F16040F36E2C9E8C11E533CD8933FA256
          93481AD3A42F79796FF7ED3E67F73531CD3503F366DFECCECCF7EDECF85CD7C5
          30E2C790121C74DCBF726E994C56AD3F074F520589B3E3065A7BBBFDC7CAA4AB
          379F6FDDF1F553D85CCB6E904973BB83B73F388A6E128B8922F6AB25281F2355
          221847B36D972941AA87E0E193A7B7BFD693E95EB6281023F3E0D3387D670EA1
          5C1AF9A0CC548FC2C7972B5B178EB9B373130E5C5762BB6CA160A7B098DA058F
          B9E8B49AB048EB951DAC59733A49D51FF7EEA0B07E77D9321AB3FD150AF6292C
          A52A306A65EF900F60660B93C913C077E0978CA11498CC7711645333E7218403
          29381CCE803D607422413DF213770B0E69788CA8103A25EFD869A4C7B6E77B6D
          945252B0807038A4E37817251804B7214985C334B5A219D17B57E35F201A07E8
          5CE94248019702150A4108EA0D8B2CF45A215289BFB547B0514DE8209FF1E7FF
          3B785F3D9AFFF9DBD270EA8D4E6FE3C61BD577228FD1038DE2F2B489573B91EE
          919C4E70316ECC47A7E21AA6C322D46386EBAF2BB87749AD6D42C0358D56F32F
          FC2A1F74821CBD818C4E609BFB60ED26055BE014C099874252A0D9AC691A4AB9
          652274248CE970FBC5B5C79B0B87664173259ECA4AD505E5D3FFDCEB0EDD5117
          F6B3D5470B83C394336A255854CDB15ADA79E638F241AA160C85110884100A7B
          B033B7D63383B3E31B769CFF093000C2D4335731DA6CD30000000049454E44AE
          426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_40'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002434944415478DA
          A4934B68134118C7FFD9A6C9EED66C25A004A51404230D16C1F6E22D4811AB82
          78D01E44118AE422419016AAE664299A8348114144F0522F5E239EAAC57A108D
          08068290A2A48FA4E69DCD3E6677F370779A84C43E2E0E7C7C33DFCCEF7BF18D
          ADD168E07F96FD5FC3EDC58D51535D30C5FFB57665D8B2655632B1DED4DC92B9
          8DC4672F473BDFDB3A3318FF311E1873448284683E4525887B26412A04E2A608
          3D3A836CA1182F14CBF3E2CB5BCF5B0CD3DAF897FD01D6C5865DFB7A7C2CEB04
          CFB118FCF594C2E21FB1F5CCC7735CD83DED0E743968A61DE4044E78675C82E9
          845EF6F11C86C82B0C6A2FE8996759B87C0F0507EF089A4E46DB3DB87EDCFDE8
          06FBC46777DAC1080C5C4603F5FD4E18460F6A4739BCF95E4724978771E86E3B
          93669FA2D4C101DE7E92FF2DA2574A37EF63EDBE94258273DEB330D4C378BD0E
          68B2D6AEBA9D01BBF24968945691AD019572111A51A16B0455DD144D43752D8D
          F3A727F1BEE441524EB61C0CB77B602BAD3279626C830D55814164C86B3148A9
          0426BC13D0651D95C414B48D7B5B4D943E2E7CB0A2A88AB40DB6343D9BF7C56F
          6F71A2FF62E708D03A1999A8FE4D59D91526EA96BDF0F333A5ACE8CDB5447BA0
          48A2096B7BC29694C54A67F4B835953483643617DA0BA6512505C2B1532DD89A
          AA7979E10E1D69C61F989BCDA7D777852D6D95373032862F8974C6344D9B70F7
          28E752C990582AEC0897CA22BC67AE91EAC1A1E5FE3EE74C27DCF5991E5F1DB9
          6FAA0714ACD6A9CDA8D5A168D5D091014FF8E6B3457DA7EFFC57800100FF9879
          CB3C6EAD860000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_42'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000018C4944415478DA
          C493BF4B425114C7BFCF9FD4225156103514080D4E4A4412042DD1D4D620D192
          D41FD0EA943838B4150D4110390411425B185643F81744D0506268F5FCF5BCAF
          F7DEF55D7DF99E18523E17870E1CCEFDF5F972CFB9E7729AA6A11FB3A04FEB5B
          C0D66B33925D99AFCAE24E5914D69A31517CA447C9502EDD53E0EBF6F4ACEE70
          069D13B398615E24F80B908C8A1A696C0A4F757BF348DA34051D96352DC84B0A
          4A84E041B86BC3865BA86DBD670D74B8A8A82042198CA998523D3F30C9AAF0BB
          7DD6D44974B5AB80781F4F314A214B22A8228314F3F0B34578E539A8AF762C0C
          05B0EC0940AAF0BB9D025CBB0F3EAE8FB53C69C135AAC06A7762D0E5C6806B18
          75C6507ECF82CF3E83943EB111BBE4FE145112AB4D981A30AB299085122AB917
          63AEC82D51DD2B42B57B0D327C21DC865559326227AC1B1125280A8D771558DA
          8E468AF93753588F7A7A63A323E7A6AF50C865C2D54AA92BAC5FDD61B3844387
          3757A68DB475908CEC077DFA70CF0059C35857EB0D489485A727C763BFFB80FB
          F7DFF82DC000AB8E0B9815C6A76E0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_44'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000BD4944415478DA
          62FCFFFF3F032580898142C0824FB2E5B187C5A7EF5F32DE7FF91800A437BCBD
          F673C69ED46727883600A4F9E1EBA7F19F1FFE66F8F5F95F3CD00090F009A2BD
          00B219AA198CFF7D670C20290C40CE8669FEFCF837C39FCF8C1B70860136FF72
          0830CF00391B64F39FCF4C1B80F40C9C0660F3EFF9896F13184251FD8CD3005C
          FE2514132C68FE8D47F897690331310137E0EBB33F58FD4B2826E0066C097D7C
          029B7F71B98CA88484CF6530C038E09989620300020C002CDBED92F9ACF53A00
          00000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_45'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000824944415478DA
          62FCFFFF3F032580898142300C0C60C125D1F2D8C3E2D3F72F19EFBF7C0C00D2
          1BDE5EFB39634FEAB313441B00D2FCF0F5D3F8CF0F7F33FCFAFC2F1E680048F8
          04D15E00D90CD50CC6FFBE33069014062067C3347F7EFC9BE1CF67C60D2485C1
          D7677F66809C0DB2F9CF67A60D407A0636758CA32971101800106000E6D95294
          988F23F20000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_46'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000021F4944415478DA
          62FCFFFF3F032580055DA060EF531320E503C40EA7FF86E982C45EDD797599F5
          59DB012073CBB596D033C8EA19915DE079C933DD856D4BDE8F1F3FB5BE7DFFC1
          704D2299E1C7E71F0C9F5E7C62F875A692E1F5BBF7D7DEBDFF38E9D3DC9C9930
          3D4C3086C36187740E5E8E2E5E1E662D0E0E76062E4E0E06F97B53C09A3FBDFC
          0453A6C5C5C9D9255426948E6200D4D9799C7C9C7CDB7F0732000D014B727371
          3268FE58C020FF733698CFC5C1C1C0ABD5C1C7C6C5960734C4041E06A6925C99
          09D213B458D8591898F89818787FFF67F827C0CEF0FB3733C35F554E868B4FB8
          18765EFACB70E84B21DC25D0703A0336C04E9627E8FB7725866F4FDF8065DF32
          DC8787CBBBF79F1914C5441914847980063030FCFCFA13EE6BB80B3EBE7DC7F7
          E7DB57B0E25FBFFF30FCFCF51B4CFF02D24F5EBC619091FFC6E0AEA5C2B0E821
          4A84E9C2C300A899099BE6EF3F7F31F07273325CBF7A9BE1DFBF7F0C014A010C
          BFBEFE62F87CBB94E1E7D36A4820EE3B7AB119976610FDE7EF5F86CF5FBF335C
          3C7F8DC14F3E0FD90597612EA8C1A7F9F7EFBF0C52E2420CD7AEDC02EB02D90E
          0507E05EC0A7194483C09B779F906DBF064A9530035A9E02030A9FE64BD71F30
          181B6BC334834C9AF47569313849333959EBD782FC884B330C989AE9326C3C7D
          FB19905906D48C91945B1E3C7E895533C87673539D1FA7EF3C5F3F7DE7797F64
          CD2899A9A67BD17F904BD0FC0A064B279532E2CACE00010600B5175980DD25BC
          9F0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_48'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000F54944415478DA
          62FCFFFF3F032580898142C082CCA93EF6E6FFEBB7EF31147DF9FA8D6159843E
          234103409AEFDC7F8CA2E0DBB7EF0C2F5EBDD9CC10A14FBA17609A1FF4C5FB81
          F8699B6F6F22DA006C9ADFBEFFE84B9401B834BFFFF089B017409A3F7EFA724A
          4941763636CDC18BCEFC0761AC8108028282FC0CFCFCBCD2BF7FFF01F3EF3D78
          220E8A052E2E4E863317AE320809F04114C6996077C1CF9FBF18989898A425C4
          845341FC3DB98EE63CDC5CA7402E0369067A8B1184F18601C890F71F3FFB46AD
          B8B809D91092A2119B2140EA1449E900DD10A0D3CDF1A6445060DD7BF018C3A0
          771F3EF9E24A898C039E1B01020C0078169F458917EE4E0000000049454E44AE
          426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_50'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002C74944415478DA
          A4534B4F135114FEEECC74A6AD0511036D9596022D2221B00013636214756742
          DCF8130C71A989890683688CC60D6E58282B134C24BA54B720890F203E782440
          78B5E551040B6D29B4D3E9CCBDDE19ED02D49537F9929373CF77DE8730C6F0B7
          D7D55E13341282482443158A88DCF57461EE6F7664BF83179DA1AB81FA739D4A
          49B05C7695094414407737B1BE341ACFC43E77B4DD9EEBDD43301D98B879A9AE
          F45DFFE5D9A98FCF587C6D9C519A628C6E5B30E5C48F713636789FBD7D727CBC
          C0B182178481E717672393AF18D5238C1A4B1CAB1CB1DF58B574861E66F35F1E
          B3D73D9563059E6066F1B2BBEE46A9B729E4ABF581D1389697E63032FC09DF63
          D360468C638D6303E07F81869328F7B734F5DDF35C31B9968383C545D72B6BDD
          30B4158E35243657D17CA20D6ABE1291C5796C27972CBD09AA2D23D8508F9262
          A9C3E44A77DAAB6B5B4F157B14690D7A46E369C9482775489284AAAA1A689A0F
          33D31350E87B78DC02644985DDB60E8743F29B9392B2A99CD32EA7914BCE80C8
          D51004052CAF98ED3587045996D1D8D482ADAD2ACC4EF420E04943625138ED39
          C252762E132D4B35034467106D9C68D83857FC63DE56C70D02C1E075336E4729
          44D1C84ACE2217DBE629EB9A0241718108766E4EADE8E6D334CD2AC1961942A8
          E2004441403E6D473A2533EAB23984BBBDE159DD202B3BC91C88586EC1E92882
          AEEB58E4CB171EEB4385730A7EAF1B36C5CDFF3DD849E5790C21C2B773DE9AC2
          C6A6FA281ADE452EB3C3B3F0A2D2E7C7F0603F44751A7E5F000ED7514B6F42CB
          EE22B29042264D1FEC59E537DDF593DE406B434DE359380E857824BB553721BF
          4A61860A35318F85C9012422035FCF5F9B69B6C65868D2874172FACC85D1A15C
          5E6AF4853238EC0942544AC038DF509348C523084F8F20B73EFCAD40FEE73195
          1D39764B76052A98E422924D04CD6C31AA46A3A9F5B987FB8F89FCEF39FF1460
          00D5F28A7F3DB969020000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_52'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001C84944415478DA
          A453CF4B1B41187D339B4D2331368D127F517BF0542A44F4504F0A3D48A07AD0
          5B2FBD29FE05FE2705A187E2A1B416D183370B3D7810A495FA83E66220440CD4
          60D4A6EB6E769D9DAFB39BB8662D964206DE32B3DF37EF7BF3BD194644686570
          B4385A2688DC4CF63EBE3CE631C70922EA68F136763898FD3CF34F06AF075F3F
          8DBF97769164AD40D2CEFB70CD1F543E583CDF599E9AF372EE83AF809C540A11
          1DE49642DAE23DCF1F6991A3D9ADA517B39AAE43D7A34138AA57D6865F6DBFF5
          096CCB549B2DB8D76721754CBBC6C8F47CF6AEEA72B1806F5F56E43050271042
          408ADF1056E9E6F8281CDB38AB5C2233944634CAEA840D02479655525313A52B
          40E20AAEF53320289D10C6261690DB5F47677B19DD9D04AED53749FB226C2379
          041E1CEB166A9DE8486174EC35B4C42472F91844CDF463104ED846CE39186960
          F241C31A25976E4593F4C0FD38F3FE911E2620E965C7141E36B5AA86EAAF0A72
          BB1BE88A5FE2D9134D154A3662D61D0255C2739445BA0205BD6913DFB7DE21F3
          B45759D61D76879BEAC39A15485C9C1EC1B9AA0649C93685FE048CAAF1D7E533
          0C5391701910F43DC687DDCD37AEBA59ECBF1E80AA3ED0E7ACFAD3569FF31F01
          0600276CDEAC9DEAABD90000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_54'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001F64944415478DA
          62FCFFFF3F0331E0F8E4ACFF0CFF4F81D9CF9E7D06D3FFFF683330820C2898BC
          A606C837C5A539D6EA9D9FB1711A42E07723C3E6A6150CBECDD719194006E44F
          5ABD1148FFFFF3F72F063E7366E6FF3F0F8EFEFFF2F53B187F38E9F5FFD8A44C
          9072B05E26649BFEFDFB8F822F9C9FC36020A2C3F043D4082CFFE74A30C3B593
          F20C3D37044FC0F430E172F6A58B73B16A36C99C8CA20EC50056166638FEF3F1
          1FC33F09150626E6FB0C7F0F0532ECDB22C660993B0D2C870C58600C989F40E0
          CB2E6F066DBBA50CFFD9D818BE5F99C4F0F1CA230605DE470C677299191E48C4
          0055C860BAE0CF3F06869F7F21F82D50C3EB69B60C97F2F81918440C19140B53
          19B40DFE337CD22E6608AC5C80DD05BFFF83343382D9023957205EBAD5C42024
          7987E1DD8A590C479F8731D864B631FC450B2BB8017F7EFF65F8F10B555A469D
          9DE1DDE276867D8F1219AC731A187EFCFCC5C0C9CC8ADB053FFE225225EB9B5A
          864F671730EC7992C2609AD90297FBFB1F870B7EFDFEC3F0F5FB2F30FBF3E3B5
          0C9AF757336CBD17C3A09B500717071BC0C582DD80BF7FFE32FCFC0551C87272
          2AC39697360C9AA9D50CBF7EFD42D1F0EF2F0776038405B819F878B9199880E1
          B8ED9B094340E30CAC4EE664C6E285AF5FBEECF74CAD4412166498965E893367
          0A7332DD82B11989CDCEB8004080010076DEFB26385E4CBC0000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_56'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000022C4944415478DA
          8C92CF6F124114C7BF105A4B1A5202C1D844A4B5A0D66A93525412FBC3D483F6
          40F54EB526C6688C7F800739B5F1E8450F44EBC5B457A3D4C4A41A2BF5679A9E
          0C41F4A0457E54856D815D761976619D2129697183BEE4EDCB9B9DCFF7BD9979
          3A5555D1683389B3DE82245CDB14F2E7697CC24549F0E595F44768987E7B527C
          FD688EC43FA93DE5A31FE299D4542AB6D9C145C8D440C9BBB4180C989A0A3058
          5255BF202B785F08838FCB28F3D59ABFFDB2DA66B3F7BC6A2AC060AE44A19204
          877CA80EF309191E9B1B2DBB8C9E07D74F1FD7141096E7971442208902F88D9F
          385619413FF1425E6BC590651817C62E419109FA4627EE360AE8D825FE5A9C55
          D779018456D7195A6134DBD0D66E46B5A2209F49C1623F40BBF906229B30303E
          A9DB2E60601F51285098A04C4A50F81C72E93528E5522DB776F5C179E20C7EC7
          1E83176DDA7710CF6403359842B224D6E1DD2E37BA07C75029FF40918BA1EB88
          435BE0D4D5DB33DC7A7207CC2AEFEB3F0955E42026EF43E4BED263BC40F45930
          A4F90AD9743C50C86DC0E2E8856BE81C862FDE44A76B0FCCD67730858BE81DBF
          01632802FBE1A44FE21EFA765CE296DDF10FDEDAEF1E99B63B3B613FE8ACB5CD
          2A3358DFE241555EC5E7E7F7D8D637D4C3D457745AA35C9F0D5A49C97F0FB10E
          AA937EE8E7E6C18FB6C3D0D13D61B45E5EF86B941B8D6D4A44F72E3088556691
          E55B70CD5807FF72313BEB8B3C9D5A66B1F1DF7F0930A702D35AEB4D8FD0602B
          5A8B7F041800D71F7621FC8A1AF80000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_57'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000011E4944415478DA
          62FCFFFF3F03258089814230F006B020735A1E7B587CFAFE25E3FD978F01407A
          C3DB6B3F67EC497D7682681780343F7CFD34FEE98DF7FC6FAFFC8C07E20C98DC
          D54D09BE040D00D9FCF9E16F865F9FFF81F1BFEF8C0148D266C4B860034CF3E7
          C7BF19FE7C66DC0012FFFE762EC8767B288DDB80AFCFFECC00FA7BE1C75B7F3E
          FE7AC5B410E88219D7B6CCD8F4E7E3FD4D40695B20DE04E223EB61C497904036
          8234F31EFCCAF02F269A8169C95286CFF6DC0C2CFC8A7E9CC2C99B711A000D30
          909FED41366B7AE63030B19A30FCFB7D86E1FAF62920258781F820109FA2D805
          78131248D1E36B329B419A40368368101FA6190C402E2084BFBD99E37B6563FC
          21108D2E479401200C34A0199B382979E114364180000300723DFE8D5774E7CC
          0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_59'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000026A4944415478DA
          A4934B68135114864F6A6A6CCCA489E9A4A489D0A4268B08B50529085D89D6C6
          4DA0A2B4F6B1B33BA75284AE5C892BBB48145C2A928A8222A408A159C4070A59
          1515352FAD264D932699A476625363E6E1BD3393B1585D8817CE1D18EEFF9DFF
          FC7746250802FCCF52FFFEE2EE93B8279AC8529BD57A6F61A346D25FB75B34EA
          D6AA5EA749BABACC376FCD0D2EEC3CAFDAE9607A3E1CC8AC33DEA1013BD1EFEC
          04D741133C7EFA09A2EF7390AF7C837C99A95B487D24E21B39BD0B307D3D1CE8
          686F3B3B35ECD66021C7613B7E685DF0C1DC9907C0F300345383F86A896BDBA7
          0E47FC12A4056F7716E39E7481F18E9F94C43C62A2F3D0989881DBB34B00B2C9
          0384165C36724F8E668E5FB8F6724201449359EAC4513BE1B49980459D713701
          5178E40E1BC47AB1D066D469C16C2034A97CF1A202C081F539CDC02211878A45
          005C9C08F9256EA2CC461D305BDF5D0AA088D23E64EB00EDB04374C0B2820C00
          486536C0613580C366806EAB117C8B63D0EFB240BDC112CA35623A166E9F9F81
          0627F74162DC7D767200058A9C0992AB4A9182A1633D108A2678C581A95D5B8A
          A56960CE510824482E9451D05390C6C1507A848254B62C6A14806EFFDEB7CBC9
          7551881DB0C87BA3099243C5C5C9A1BE5B290081340AA0CF6EBDF17C395D4D64
          CA8A889383E4E52CF00858BC92ABC0AB375FAA58A3002E8DBA431692083E7A16
          AB27572B12805381E1A11F1CA33DCA756271F045ACDE45EA8397C7DDA15D9FF2
          D8D5A540AEC478078F7413871D9DE0B49AC4403FAE9545DBB83316DFBF726AF2
          8FFF025EF3F73E785E7F5EA3AA5B3F7ACB9B35B219329E19DB6E76FE2BE05FD7
          4F01060093DF5E1306E4C8A50000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_61'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000CD4944415478DA
          62FCFFFF3F032580898142C0026318E60B839CD202C43530FAFCC4B78C441B00
          0431407C1A886F20D10401232C0C5C664A6D0252BE44E8D9BC27FD991F860114
          8741D442F54DFFFF32F8FEF98130F0D7D77F70F6F74F7FC1B6EF6ABEEF87D580
          DF5FFFFB2647A5327CFDFD85E1D3AF8F0C5F7E7F62F8FCE303C3AF5FBF197EFF
          F8C9F0F3FB4F86F3DB9EF8E274C1EF9FFF188E5D3AC2F0E7CF1F86DFBFFF0035
          FE64F805D4F807C8FE09340444E3F5C2B7777F371FDD7A8550206EC6190BA381
          388081081060007A72B5F10F965F230000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_63'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000009F4944415478DA
          EC52390EC230109C20FEC26BCC03D2225EC24BF8021614D4408B84E869A81198
          38F17AD7314E0329388A34149966668B19ED68378B31A20B06E8883E0018B687
          7C3E5AC40025EE75196FEBA7AE4C6848AF67A7F1DB00B6514DF2292C1730FE86
          820DEEEE0AEF19EC085411F6ABB3FAB801538DDD61031101B32423C127A3244D
          29A4E1AF15CA4BD0DBE551FDA8ADDB43D67FE21F043C041800BD055238A75038
          CC0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_65'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002B84944415478DA
          94524D4F1351143DEFCD94B674CA94960212304A600521881B14E31EA3899088
          E24217988889EE588889D1F8078C2C15174262E2068DAC408468440413451401
          1BA50A0524A5ED74CA74BE3AE3944209362E3C791FC97DEF9C77EF3D8F98A689
          349E0E0CD46ABA3E7E777471DFB5E3159BCDCDCD88487A7862F687F6F6F33735
          7D8732ACC230CC7261A1E7BDAE2A432FA7BE4E92B440DFA33E812BF0CC3294AD
          A79475B22C452AA5418CC7D07EBE1D4B9B0654C3842C2B888B09446371FC0CAD
          E15EFF600B53575757CBDA6C8D8A621E999F5FB06D6C44A1AA3A5CF96E80A4E0
          F7FBC1BB39C806455E9E0D6ECE85E222AF75EEC0B3D733E7684AD727EC76AE3E
          1008A062FF01F1D4C91370711C924A12829040EF835E3819E4E053300CA22BA0
          67DADADCC9A4E2F4F9FC28F215DDAFA9AD86CFC75B2F70300C06DD37BAB70879
          C444525631B7B886C7C35378FEE2CD569CA93C582910CA180E87D3B6B999381C
          5A59652311011EDE83406001232343A86A6842FFF8778C7D0C60F4DD34E682AB
          082754689208AA695A5324B23E4D2901CFF3F63CD68E627F31666666E0F3BA71
          B9F38AD57D06F92E0E2E7701787F296C4E0E84B16D65C0765CEAF862B950130A
          2D8E1B066D48BB2059CA5E6F01965796505A5A8A57BFE44CE1E6F604C9F682A6
          970B172FF0AA2A77262541EBEABA0A4D97D0D2DA8A5BB7EF40D40051357698DB
          5C332B41763ED20EC63E2C9847EBAA4008C158504699371F8B6109C1DF11ACAD
          47B0B21EB6F60D84233124D68260FFB6E7E6C3411C6A3C868AB2125497FBB19B
          3BC90C7337922DE15F30CD6CA25996B937922BC0DA1D904421738398B9AA24B3
          18AA0CD32A93E69E931E31BA81F5D0B2C527D89B70064A226A0948D6F36C4F4E
          0FF892F2EB5A323A2C25C4B3139393A7270C9DC604C1F2817231418C27247925
          A5EAD3662AF5C4707846735CF85FFC1160007DC1390B303866F0000000004945
          4E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_67'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000008A4944415478DA
          62FCFFFF3F032580898142C0B2AD33B5094897BC7BF78EE3F3D7CF8C9F3E7C66
          F8FEE36773C39A7375C418C0B8B523E5BF736812C3C777AF193EBE7FCBF0FAF9
          53869D1BD7309CFF2D6702943F03C438E94D9B369D65013296EC5A313BE6EEBD
          070CDFBF7D65F8F1F317C39F3F7F9B81E2CF80388200CDC0381A88A381383C02
          1120C000AFD9C24230BD3CC20000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_69'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000E14944415478DA
          62FCFFFF3F032580898142C0E4E7E7170DC49B1818813C1C18240F5287CB05E1
          40DC48C0A246A83A4CE0EBEB0B0C05C21043DD7F08A63C0C283580058837FBF9
          FA1943F96780D8040B0D029B71B9602510D703F133208EC04183E45BB019C008
          0A10B2C07F9817F0E967A447206EEB4C6D02D225EFDEBDE3F8FCF533E3A70F9F
          19BEFFF8D9DCB0E65C1D3106306EED48F9EF1C9AC4F0F1DD6B868FEFDF32BC7E
          FE9461E7C6350CE77FCB99E0891530BD69D3A6B3A0685CB26BC5EC98BBF71E30
          7CFFF695E1C7CF5F0C7FFEFC6D26102B301A180B14666780000300CEB37C46AF
          A193F40000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_71'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000008E4944415478DA
          62FCFFFF3F032580898142306A0003030B32E7C8C60BFFAF9F7E8055E19FDF7F
          19188011A6662CC7E01C6ECA88D5808B876E3384163833FCFDF38FE1CF9FBF0C
          FF80F4DFBFFFC0FCBF400340624B276C021980DD053F7FFC66B87CF42EC32F20
          0DB2F1F7CF3F286C903C2B0B3B6E2FC8A88831EC597F04AF9FADDD4C51F88CA3
          2971101800106000CEB6494485BC5FAA0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_72'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000784944415478DA
          62FCFFFF3F03258089814230F006B0444E3F405120B040694632F5FF0719C0F8
          9B4B14C515ACDF5E132D060E83FF7F7E3244992B8031884D8A18D80BFF7EFF60
          78FCFA3D038C4D8A1863C4B4FD0CDFFFA0A6264E16464662C5402EF80F62A087
          0E9162FF61B1407654328EE605068000030000905666418C81FA000000004945
          4E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_73'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000624944415478DA
          62FCFFFF3F032580898142300C0C60C12618B2E81AD6905D13A7C5489401FFFF
          FE6648B6D740119B7BF006F12E0019F0FECB370C31E20DF8F38BE1CDC7AF1862
          2418F09BE12D8601BF4933E0E29DC744C502E36852A6DC008000030097432FE4
          49ECCF950000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_74'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000003284944415478DA
          64534F4C146714FFCDCCB7B3CBAC3BB0B0B08222555245495A13BD20D86A0405
          8D8DE241D370319A1E9AF66282692F4D7B6C7BE9A1F1603C681AA57AA8D1265E
          8C17D3722086343D103506147017DC5DF6EFCCCECC3733CF6F6631D2F47DF9F2
          BDC9BCF77EEFCFEF4944848DD27F79261563F2459FE8A8CDA9D5743CAAD95E4E
          983D14BFAF16AE0E9537DA4B1B030C7E3F7BAE45635794642C194F5A58AA6A58
          337DB84E1EF98C0FA98CD7C2EC6B11E4EE3B1FF99D32F4C3ECB5CEAEF8D447FB
          E4E499834B48E92A5AF518743D0A379A44A29B23F1417E8BCFF81F6D5FFCF5D3
          7F02ECFDF6C937898EF8854D5D09A412C0F272277455436B9382842AA359236C
          4FCDE3C0DE0788A44A20989322C889B084EEAFA6DBD2CDEAABF6FEF6F8F69482
          EE261B3B3A6CBC282490B31C707F012DF13F31B73A874DD166FCFDEF51E41752
          605E7B2E6819F3882EDA7A34EE334297BE884E9D43530DF4A52D6CE6192C575F
          622EFF1C4BE54548BE8C5D3D0A4A99315085DA25D6F139733D3A5C8F30F4B43E
          15F55451A117B07D065F715170725875B3303D031EB9E08E07CE2C30A50A311C
          8800A38CBB6EEF8EADCFD0A63DC2ECFC303E4CA7D01475202B3514D5222A7609
          865D03F7383C9F4092095E9541BE03226F1B239F3B93FBFAB07FCB41CCF72EE2
          C7C759ECDABA809AC990ABAFA260146158266452207280EB39A2741EF65FF8AA
          2C22F1A58FD3DD7B8E1F1EC6FD47F7906C99C3D8B6B3D8DDD68F8023E1210F4F
          32FFE0E799EFF0FA590F9A6403962AC1F31CD1B40BF72F654B751A1818A0BC99
          A39CF9860A6629FC5EA918B45236423D5BAAD16AB9B47ECBB45234E8FCAF33BF
          C83B37C77E1764F083999E3CF219A6AE4FE1F6CD9B21494E1F1BC69DA95BA13E
          3E36223291716A744CDC51DCFAED3AAE7DB9FF5C9066A46EBB6B014AC12C8668
          6F2AE5066AC5A26CB9915D5620668BB586BED67885644226C654C50A5E6E2921
          9A185083A7BE488CFC75D2BEDF192D1A31FFB70B81E8F15831F4E312063FF954
          A47D44F891D00F61FCF888D8BC46A9A3C387B4898989DAC66DBC21EE48E84C62
          D2168FDBDC5529283A4091245F8D305B8B45EA438383E9E9E9E9EC3AE689B702
          0C00CBD99A8868CC5C4A0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_75'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000010D4944415478DA
          62FCFFFF3F03258089814240B1012CA428762DD79103523381D803887700713A
          0B8916CEB1B0D37555D29662387BFCBAC7CD0B8FE63091E8150369251186F71F
          3F32708BB183F9208DE640EC0BA5B1850D32FBF8A573B7195EBC7CCDF0E8D60B
          301FE405092B2BAB0DC78E1D0B802A360589A1D90C527D52804728F3EE95E7BF
          FFFDFB0B52730A88731980E9C0DFD2D2F23F888661643EBA1C121604A5217820
          825CA1A3A3B38A9595F50F8C6F6868B80CC606B9104483F82075B366CD023137
          A2B8009DC6C5461263A24D4A9496963E02732E321BD9AB50EE3F46A033CCB184
          3A5680145B20700888DF33427323C825FF9068062C6C5F24B336C2188C246667
          6443C10020C0003F42AFF8CA40B3980000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'mainmdimages_76'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001794944415478DA
          62FCFFFFBF390303830410BF00E2930C10C004C4FFB0B031000B48B39595D586
          63C78E0540159B420D440630C399A07CB8814C680A7D6106C204A06C9081FE50
          795F285B10E602B8421D1D9D55ACACAC7F607C4343C3653036C885308341EA66
          CD9A05626E64008681BFA5A5E57F6C342E36921813130391E0F9A5C5A6D8C4B1
          1A202D2D7D04E65C18FBC7C787AAC85E850526235234E204BFBF1E33BD776445
          E0ACEDAFB47A27AC08800A1F02E2F7A030006126341ACEBEBA79FAA64F77ABFF
          5FD918FFFFDB9B39FF6FED597C0A490D46BC2227987FDFDFCEF595D57AE2CB7B
          F02B83A6670E03E7A62B0C128A374C41E23045202F6038F9EAA60490023320B6
          07625B9066265613867FBFCF305CDF3E05A4E430101F04E253580D8001904D7F
          3EDEDF0472C1BF986806A6254B193EDB7333B0F02BFA710A276FC6190B300052
          F4F89ACC66902690CD201AC487690603E400C1858181E70B0CC443201A5D8E28
          0340186840333671A2532228C0B00902041800778333C92E59ED2B0000000049
          454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001604944415478DA
          A493BF4AC3401CC7BF1541295428E223884F2138E822ED1B78427177F4017411
          71727015EC1B94A05B1797EE52AB36426B8BF67F6CEE1A93BB68BC9C83EDA5A1
          426FCB975F3EDFBBCF2589200830CF5AC09C6B71D6C051FEEE9A3A0E19CF52C9
          64FE8C6CEEFF0B60D98C9C1FECA0C3B87AAED4DF51283D87C03F40D8D2B5E844
          CB5A3AA55A5CE1A3D663A85B0E1232AF776C7021268FF0D6B5C8696E1B8D8103
          48A715F305C5725BB5B81E07755CF42DAA5EB02985EB69004F0E3D367A78ED51
          D5D2EC0CC1F9EF963D2E706FD6506B0F81EF10604BC09706E01E281B61607DA8
          9032D922B37009E11B859299D1D4181A80E3A16AA2D91DAA233009F0C4B21AB8
          3DDECBCEBCC6A505E48BE516198F8351EB297771A37F65C6D5E16E36025849AF
          92CBA8C48D13B2A56799A93B88931827360A88911827760A60BAC438B111802F
          AF4A4ACC8CC79FAC5F2D96FDF5C951DFD00189797FE71F01060043310BA9F216
          C9810000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000C34944415478DA
          62FCFFFF3F032580898142C00222F27B16745918EA24C004CF5DBE7EDF485753
          110F7F7377416C32DC8027F76FDB8625FF12FDFB6E32C3A72F920C6B6E9BDEED
          88FF8E8FAF85E2021078FF680D90946478FD561585FFE1A32C563E8A01320ACA
          0F77BF095486092AA95E6702F25FE3E13F839B008A85A0ACAAE3FF9100917CB0
          5E8A6361E00D80C7C2CF7FA81284F818067CFD8B2A41888F61C0C73FA81284F8
          30C0088A8AE0ECEA4A20DB8804AF9F5B3BB5B51D6EC080C60240800100BE679C
          A21D78DC430000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_09'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000CA4944415478DA
          62FCFFFF3F0308382455DA02A974208E06F10FCC6B6764400240F9FF50E65220
          9E09943F0CE2B020A90169DE0C948861C0026006020D0A87AA051BC000720108
          DB2756FC87B1096164B54C0C1402AA1AB099047D9B510C00068C37903A458201
          A7A07AE0B1B00588754830603D105F01624698177C803890040302A17A50A271
          1309D1B8095B34FA92E0025F9CD1084AB240DC8C8F4D281DB420C5082E360220
          F96B09108713E1FF70905A181F3933CD046512A0335710931B61E28CB0EC4C2E
          00083000A20DBEA7101B7B2A0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_11'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000B44944415478DA
          62FCFFFF3F032580898142306A0003030B32A77AD6C6262055EB62A2C9B0E7CC
          7506647AFBF14B0C1F3E7E6C9E55955C876202281A61B864D28AFFB75F7EF9AF
          6EE3F5FFD5A7EF60FAF97B08FFE1EB0FFF43CBFBFF23AB076146E47490DFB3E8
          BFA1A101839E8228C3F1AB8F184CD4A418F69FBFC360AD23CFB0F9D86586678F
          1F332C69C961C4E9859F3F7F2ED9B97B4FCC4E28FFC831087DF1E27988621696
          66F430601C4D8983C00080000300B68C63788C6DB5EB0000000049454E44AE42
          6082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_13'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001464944415478DA
          62FCFFFF3F0325800544144C5E5303A44C89D5F4E2FEF52B2BFA6AABE1068034
          4FC80DF1FBFBEF1FA66A2C0E0CCBAB154371010C303331C1D94F5FBD6578FDEE
          238A4651217E063121014C2F6003D262C2608C0E7EFFF94B9C01B85C806E28DC
          00506CFC03F99711E26F0911413086F021828C40881E6B7003FE00C3EF1F4812
          8419814AFF434210A2F73FC420209F05299C500CF80D54F3F4F96B86371F3EA2
          863CD430A0510C2282FC0CF2D262D80DF8F5EB2F8308308445E0A1CC88169760
          0F30FCFD83C30BA0B07DF2F20DC3BBF79FB0872AD03C61017E066519091C2EF8
          FD874148408041985F00AC181204C0806364040701480CE415F4C4867001307E
          9FBC7AC3F0FEE367988B3180203F1F83A61C0E17080B7033F0F1723330A169FC
          8B96943999B164A60BE7CF3278A69EDD446C6612E66482470523A5D91920C000
          69B0762D7303DA760000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_15'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000023C4944415478DA
          7C933B6B145114C7FFD99D7D3F494864D53C202149215B5828461045D1226914
          6D14EC2C530A567E03BF808DA511B5B01034288A602C02B21A89AB108C51D935
          C6B0D9D7CC7DCD7AEECDCE9021B2331CCEB977E6FCCFEF9CB9D3D7E974A0AFC2
          F9E20972636487C8D2E87D7D21FB5559FCF8C6EA265F2457BC3E3F7550AFA356
          0CA3B909F3E650AA60FCE1EC98F1E3FDD3F8B45E9DBB7CF3C61DBD3602939383
          572FCC9C3ADEA86F0C0BA1B0C31596CAABDD62AB81D2078632B83577FB2E8553
          BEC099D313D593C74686FFB4DD9EDCC9481E0F9F2C616DBBECEF1901D7ED1CF5
          368E0C9EC540622490B8D5DEC087DF8B10742BA502CFAC5E15BD012B5781738E
          B0C520A5EC2DB0B2F982125CA2D2A62095366944A022E014FFACAFFBEF87F656
          7BBEB08C67F79751484DC3E10C367376CD71D0B26D687A21F87E02A904B814B8
          746D16B1501AC9709E926CC215864050551DC7FB18B810FB0504F5C549F9074D
          576A6C5AEB3D8D2E4C2C4CDC1FB7884060B35509B6C008973186978F4A78F5B8
          84D17C9190DB68DA2D34DB2D8AB56F9A76B8E4D8B6B782048C26AC7B9EBD720E
          A9480EE9E88049F0083411A7361DA65B9068F05A50C01BD8D76AC9207A3D0BDD
          BF9066465A8C71C73CABD97FFF2FA071FD6453B9FB09D5EE3CB8600847E93CB0
          8CFE415EFB02F51DFB3309CC24AC0C6DD0F707A187246289A43979A97816AE72
          0D3EB3D5FB07F7CAF3F4277EF7059239F1B656898FBF5BF9966D37DCB44B67A2
          63CC355EAF2371D55422B42698F5D44BD6D73F0106006BDE83A188216C5E0000
          000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_17'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001124944415478DA
          62FCFFFF3F03258089814240B1012CC89CF8A286A35F7EFE3E8C4F030F3BABED
          C2BE066BAC068034FF17902BE7E0E064E0E1E303625E88F8A7CF40FC89E1FB8F
          EF0C5F3E3CEAC4E90210F8F1E503C3BF5F3F1818FEFE02A26F60B1EF5FBF317C
          FBFA95E117509C139F17FEB372F2647A5B32FCC511332C4C8C0CFDF396CBE034
          404E59E39EB3A32DC3CF3FD8FDCFCDC6C0307FFDCE2778BDF0EBD75F86AF384C
          E06062C31F0B20F017887FE3485BFF0845E3A3BB379476EE3BC8F0F907761788
          F0B0337C78FF0E7F18D8595A30BCFCF41DCC67443340498C8F61D9E6DDF8C340
          58809B818F979B0118E0C0D84095E36426E0850BE7CF3278A69E2DC697128539
          99C490F98C039E1B01020C00C46D64E09F6CE8360000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_21'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001C34944415478DA
          62F8FFFF3F03215CE1A3A5B67F79D73E108D2E4794E67D4B3BF6FDFF76E4FFC6
          4929188610D4BC67492350F396BF7FDF2FFEFFF765C7DFFD8B33510C6162C001
          2A7DB5D55CC2C367380598DAFFFD7A83F1DF87DDFFFEFEFFC168E72164EF1E61
          3B03240F528761002323E37F20AE39F686FDE69E0B579F35D75733FDFF76F9FF
          DFDF8F18997EDFF8CFC0F88FC9DA83D7D1C95703620846A03030D4C09CBF737A
          F8BEFF2F92FEFFB8EDF0F7C72DE37F7F1E6AFDFDF544F5FFE60976206F38630D
          0306B010220C764E30DBF7FF81E9FF5F7775FEFD7AA2F67F63B73A5C332E036A
          D00372D764DD7DFF9F69FDDFDC83AA1984591C536BC3FFFFFD13CDCA23E40BD4
          1C0BC4CDAE79DD0F80F4E2DF5FDE6D66554C58CAB0734106132BE38CA307985B
          DB375FDD8B1268F689159B66ED3CF7FFE6FBFF600C72018C0D1277C9EDDA0475
          8910B6A80619F0FFD0F3FFFF63AA7A419AE118C4078983E4F1A5157034FEF8CB
          C0109755C400D40476158806F141E284000B8878F313C2F14A2E82D330318206
          080B8B6CBE7AF2B02FBFA0305840CFD683E1C9AD6B60F6ABE74F41D466BC06BC
          FBF061E991DDDB406C5F2CF29B19995996E23380111AF76403800003001386B2
          400480BC5C0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_28'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000015A4944415478DA
          62FCFFFF3F033EA0E5970EA6AF6D9A0962FB02992B803802883783C498188800
          689AB990E59848D03C1188FB90C58932E0ED874F97C23DEDA601994B819A6AD1
          E519B185816AF28400205500C4065EC6CAEC81664A1C594D937E83D403310B10
          7F03B90664202E03E600A9BD402C0EC4FD403CE7F6DC825468A08234F801356F
          66809A8601808A53A0063900A98F40FC1E971759704900356F0052FE50031623
          051E238A01AEED5B41FE5D0FE52F04E286DD95DE0F8074021083E426400D30C0
          661113547323349AE2F5E5844FBFF9F4ED12D01B1F80780150EC06103FC0E785
          40A08D20E732844FDA93FEE6CB0F8E571FBE485916CE0A030A8901B1061057E0
          3400A619E815508071A45B28DD901315380764AB01B12110D7005D72009701E0
          68046A16808634C81BCB80869E6220123001351B403537023582124F0E030980
          D1A56D0B464A021AC448B40184B23321001060002A2286A1E63D5ABF00000000
          49454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_30'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001974944415478DA
          62FCFFFF3F03258005C62898BCA60648996253C42F2CCAF0F1ED6B06797E9605
          857101EB5124412E00E1FC49AB3702E9FF7FFEFEC5C47FFEFE5FB2F7CCFF43E7
          6FFCEF5BB83E10A6078499D06DDBB6751BC3BF7FFF513150212737170307073B
          83899EE6BAFE451B0261EA310C78F8F00156BFB27372303CFEF89DE1D1BB2F0C
          0F3FFE49C0080364C0CAC2CCF0A92C8D81454B9F812B211B2CE6ADA70897DFB9
          7B9F185617C062E4DFBF7F0CBF8EEE63F87DED22985DB33D9E61C9D97E301BA4
          E6EBA70F0C585DF0E71F84FEF9174203BD0F669F78B49BE11F549C8D19D5B528
          2EF80D4D123FFE32C20D4067FFC5950E40E0D7AFBF0C8CB76F337CFDFE93E1E7
          E7CF0CAC6F5E33DCBB7B9AE1FB9BBF0CEF79DF83C53998D8701B00325D5D5C86
          61634339C3FFB45A88E0FE530C91FC40F63B0686E30776317879FBE071C1EF3F
          0C12F1E90C127892EEDF7FFFF0B8E0CF5F86074F9E317CFBFE1DAB666E4E4E06
          190125DC06080B7033D8196A303031A2D98A94DF38997104E285F367193C53CF
          6E2226070A7332C1131223A5D91920C0009CABD7E2926F1B310000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_32'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000904944415478DA
          62FCFFFF3F03258089814280628086ADF726B20D006A8E0652BE509A2C1784E7
          847B8269929C000A44189E3C79F27F643E3198BA8138D20D7863AD12CE78FB36
          88C6999840090D3D9D30C2F20248E3EABB5F7CFF858733FC5755C56AC0FD67AF
          18B61E3E0B626EBE7178AB1F4A3A786DA51C0EC49B4034AE3857B7F1DA04C4D1
          C8628C039E1B01020C0066057E5C79975EC20000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_34'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000010C4944415478DA
          C493B14A03411086FF8B09218585601310DFC34EF031047BDF412BF30A0A629B
          DAC207B0536CAF539044A3104310EE6E776F6E762F3BDE092923B75CE15F2D0C
          F3CD3F3F3B9188A08D3A68A9D680EE5FC59BE5D1B1A162EC217899CCFDF5E174
          ABB183BBECE474B7371C0FFBFB789D2FD4FB67D269BCC2073D9CED750FAE580F
          703F7B9C7DA5C9B65F49500617EBC7B7D23DEF004F01192C7D8CD8DEE28D16A8
          D6DF71B9C019690E98F013141BD47FC491F44BED61F300C0D43C8398A188E1B4
          449C09CA1007A93120EBA0B9804D3C38A9002119A439819C03BB129CA27601F1
          326A0C5045F1DBEC56D5F4CCA30A71145FEAF3C600C316EB1BB30A1B9B6B45FF
          7E8D3F020C000BF6A282DBAD96FF0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_36'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000010B4944415478DA
          62FCCFF09F0119B8B66D652000401A5A80B816C461419755D1D0F88F4FF7FB37
          6F187EFCF85103624BCAC8D432FEFF8FE682F6ADFF1988000FEEDC06512D2C38
          247002051555B8E6DB730B6A19402E2005AB24F5FF07E266189F858148107E54
          F13FC4D6FB8CC8E2F0306879EC81D7EF1FBF7D66F8F0F51358E96CA3CBB51806
          406D20086EAD8218727EE25BD468844AE004D2365C0C4F8F7C43D10C49154406
          9E419ED07F206E4617271C8809DEE0C03BBFE02D233669C6FFF30AF0FBFDE15B
          0686CF3FC04E67E85D558B2ECDC270E52951D1F8E6C4B91A066B150691A37750
          0C617C6DA58CD7057C52D20C9F9E812D6941D74C5420822C00E2665CF2189989
          5400106000E902EDE49262E45C0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_38'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001D44944415478DA
          A493CF4B146118C73F33B39A933F5877714D12B2C3B28549125AF72E15824284
          54D0A1439740F1144479F0D82522FF880ABC640449A0A720F09051A96BBBADE2
          8F743457DD1DDC9D9D77C677375C72DC05C9079EF7E7F37E9FEFF3E3555CD7E5
          38E2CB0F03C3234FE5D479D447AB8999EFAF9F0F3E2902E41FBFE8BBD52D1CE7
          B0750982BDFD83A1030CF6C571DCFF0BE180882CCEF207843D85EA330A47C9A5
          3A54FF15EA235D285A4579800A9F463AFE9E9AC882DC9D951A909AA1A9D9646E
          F425AE2338D5DE5B1A205F0DD7CEB03DFF89D4CE86340CA3A8905B5C662DFA8B
          F5D834954D06DEAA15016C99BFF4CF779CBED6CC66BC861F6FC74925A2589689
          E6AB27D4F188C6CB0FF1A6A9089093175BC94D329353345E3C47A0E5AAF41EC1
          88C698FFF20D3319633767A395CB819D13545DB84FEAB3C19FD131E93D5EF0DE
          D0DA4EDB9D9BAC4ECE627C1DE164C7EDF20C32C2459C08120C9CE17C4F179A9A
          952C56589B4D1472A0FBD711E542B0243D73D782F03D5696C2C45FBD41F9FD91
          AC95456BB94BEDF5212A1B5AF1365B1140D8421A5B7F37A14BE852E119FA3FC6
          96BC7744556980A0BF9ABADA6A54C5D3571ECABA562204339D9EB8F1E0F191DB
          37A8AB73FB6BE5B8DF794F800100825FBDC930232F090000000049454E44AE42
          6082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_40'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001504944415478DA
          62FCFFFF3F0325800544144C5E130EA4A288D5F4E2FEF5352BFA6A17C30D0069
          4E8F12F57BF7FD29C3A3B70F1914FEFA3008C8BF6378FDE915C3D34F77E07C98
          FC9A1B3FC4807A500C004B3EFF7293E1D6EBCB0C5FDF6A33688A7D6178F51395
          0F936760D044F50208ECB9B1062E78E5D63D86A7C28770F231C200048CD9AA18
          5E3C7F06560C02EF8FDBA12804F175D494182424A5182E31AC47488062217FD2
          EA8D4FDF7DFD0F03F94DFDFF9101321FA42E28ABEA38481F08333150085810FE
          BCCFB0EDC15DB8170A9A27A02804F1415E101016C76E80A29C0C839BB9365C71
          7A96213CDA4E6EE26498505B0096BBFDFC23C3F2F59B300D4007ABCE4F44E2D9
          11F6C2C5AB37180E1E7A8933167079011E0BB79E7D8087F4EC151B516201990F
          52871C0B285E00F90F0440B6C0D8D8F8185EF8FAE5CBFED4EA0EA2A34E989369
          038CCD48697606083000FEB9FD19A37E1DAC0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_42'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000010C4944415478DA
          C493B14A03411086FF8B09218585601310DFC34EF031047BDF412BF30A0A629B
          DAC207B0536CAF539044A3104310EE6E776F6E762F3BDE092923B75CE15F2D0C
          F3CD3F3F3B9188A08D3A68A9D680EE5FC59BE5D1B1A162EC217899CCFDF5E174
          ABB183BBECE474B7371C0FFBFB789D2FD4FB67D269BCC2073D9CED750FAE580F
          703F7B9C7DA5C9B65F49500617EBC7B7D23DEF004F01192C7D8CD8DEE28D16A8
          D6DF71B9C019690E98F013141BD47FC491F44BED61F300C0D43C8398A188E1B4
          449C09CA1007A93120EBA0B9804D3C38A9002119A439819C03BB129CA27601F1
          326A0C5045F1DBEC56D5F4CCA30A71145FEAF3C600C316EB1BB30A1B9B6B45FF
          7E8D3F020C000BF6A282DBAD96FF0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'basicmdimages_44'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000D44944415478DA
          62FCFFFF3F03258089814240B1012C20C22DAD76C2FDBFC27E40262F319A1499
          DF2EDD35ABB9006E0010982BB1BEE325D6D6FFFF18CC111C6020E64F5A3DFD3F
          91E0DFBF7FFF83B2AAF682F481304A185CF8CEC0B068D122BCF4EFFF3802F1EF
          BFFF0CA2EF9E32686868E0A5FFFEF98FDD005072782D24CD70E3C60DBC34CE68
          04994B8C0BFEE333801817E034E0CFDF7FC485C1BF7FD80DF8F5FB2F8343DD6A
          301B1FFDEFEF3FB44441623AF8F9E73F4A3A00A7C4C78F1E5E7348AA9C416C4A
          E464617C046333529A9D01020C009D9DFC6ED46372F70000000049454E44AE42
          6082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'calcmdimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002174944415478DA
          C493CD6B135114C5CF4C26F3914C9D4C4B9AA41A54B49860EB077553F0EB3FF0
          0F9880EE022E5D0749A12EC4954B37AE323B178A202EBAD22E8C4815AA4D41A3
          8DA649B54D6293D6989979F37C13198841DC64E183BBB9DC73B8EFF7CEE328A5
          18E5F018F18C6CC0A5AE44FFD68F4FCC04F38100CD12827B8DB74E9EF5B68687
          4A0FBF411816C6CE287931CC6745854740E2E0F46856D14896F4C8F3FAABDE35
          36F37150E01BC49273EA425893B28A2A420949DE72E0780E608C1DD70271C985
          88EE945BB5AEB9F56EEFA66F24244FA9053D1132C6B4300EA82A444944909310
          E09801E71950D8D405716CD89A0527691B47A67F18F5CA7793E9333E83D4ECFC
          A1DCB1D441231AD5210BCC8017999683B78443095CA7879D661BEF4B5FCC95E5
          4F8B4CB3EE311886787EF6ECD1C2C54B7387E3F1DF7D2F268D9D0696965EB69E
          3D58BBB5BDB6FF84694A3E44DF202668B8ED5AF4AA20F2E0791E27A78FE3F4CC
          092C17DFA0B25905E529C84F56B6FB08367FC363D03738773D515074C190C322
          422119D6BE0BABE9F6EFEF21F02012C640D6044811F62A2E41B763A1BDDD338B
          77AB19616FD3C9B05A9C4A8BB9B02E1B530915B218640C827F32B06DECEE7651
          DF6899D5D54E9FC16012D76BA57666E5E9E774A3DC3105474254D731391EC144
          24020522BE6E74CC178F3FA42BAB8D8C2FFE571253F39753B9F1C931A35669DE
          795D2CDF1F140D2691FBEFBFF197000300E209DD954339C5290000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'calcmdimages_09'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000018C4944415478DA
          62FCFFFF3F032580116440C1E4353540B629B19A5EDCBF7E65455F6D3588CD02
          15339D901BE2F7F7DF3F4CD5581C1896572B0663B3204BFCFB47BA7798182804
          2806B0B230C3F1C7CF6F1926AF6B6178F1F6098A38082303B817408109C26F3F
          BC669877A08F61E795950C7FFEFD65D8726B21838DBC37438A4B11838CB83C03
          2323237603FE00C3AF79793EC3FE2B1B18D8B858193838381858999919FE020D
          39FD7C27C391E95B1994F90D186614AEC56EC06F60F8D5474D66487896C7B0F6
          D46C86DB0C4718FE30FD65E064626310FEAFCA60ADE5CEE0A21CCEF0172D5A10
          2EF8FD97E1E3AFBF0C82420A0C05FE9D0C8F9FDF61D8737505838B760483ACA4
          0AC3979F7F185E7DFEC92008741D4E17FCF8FB1F88FF327CF8F1978157508121
          C9B586E1FDF73F0C4F3EFE846BF88B16D370037EFDFEC3F0F5FB2FB8C4D7EF0C
          0C2F3F6046DB5F2E16EC06FCFDF397E1E7AF5F98691D8DFFEF2F0776038405B8
          19F878B91998D074A03B9993198B17BE7EF9B2DF33B592E8D427CCC9740B2537
          520200020C00E1CB97D81526F43F0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'calcmdimages_15'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001444944415478DA
          62FCFFFF3F03258091660664AE36ACFCF49EB9EDC99BE7DFFEFDFBFF03A86E1F
          507823102F05E2FF476A9F83D531E1D2CCC8C8D7C627F8D7FDEFDF7F2A40CD3A
          40E19540EC0EC4F5208B616A99B0684EFAFC9BB1EDDB972F1DD343CFEF020A3D
          87E235401C0BC4CA401C8DD380F7BFFECC7D72F72DC357A66F39203ECCA94800
          E40D7F943030CC17F6151711D9F457F01BC38F2F7F18FEFF8648FEFDF99FE1EF
          B7FF0CBFFFFEF1E31661DB0CD52309C46781064BC15D707EE2DBCD2FDFBCF1FB
          F2F82F83B4823083948A2083003F2FC38FD7189A3100DC0B20437EFCF8E9F7F4
          F247868FCF7E303C3EF7918181E38F1F481C4D8F35101FC51A0620C55FBE7CF3
          7B71F92B03133F56CD0C50FF6FC41988204D583483FC1D02C48B81F82E342D10
          97126D9A25D5809406106B02F149203E889C9018076F5EA09B0100010600256F
          ABD5350BFE960000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002234944415478DA
          62FCFFFF3F032580D1A56C9648A88B792A2F278B0B271BB3293B2B0B2F48E2E7
          EF3F9FBFFFFA7BE6F38F3FBBD71D383773676BE23B74CD20CB5904F8B9BB94A5
          04C35514A4B918999818600E023279FFFCF9E778E6F24DB3FBF71F2A0085D2B1
          B98089898939F13B1307170B50071F0B03033B3303C3EFBFFF18DE7DFEC170ED
          E12B8613B79E7303ED4AC3E505961FDFBE303C7CF38D41469C81E12650D39507
          2F191E3C7DC1F0ECE93386E72F5E317CFAF68381F1CF4F9C61C0040E0886FF10
          FFB0B030707072327071733370F1093070F20B3330B1B2031530E137001A9E28
          24282840D844559AA121D91FA7012CF8A2C840518CC1545184E1EDFA2C863969
          8AB0F85E9232EB7E2C86018C501BA111042655240418C41E4F64B0F1E607F274
          193EBE78CA70E4E0BB18202716C50B10E7FE4731444B5A90414A888781F7C321
          0609FD2030FEF6F12BA61738B878189818996129034C2989F0307033FD6178B1
          3A9D4144141103BFBE41D850EF2C01B98459C72D4C8A95E19F86300F3B1B030B
          1B0337D0537C6C0C0CDF4FD532B899FD05BBE9DCCE1D0C774F1F6210971361D0
          B350661017F9C3F0E8E1773D23DFFC46161915CD499FDE3FDF79F1DEF3DA1FDF
          EF1BF0B1FC65D0951763D0F87F0AE8EC0A20061AB6AA036CB3864716987E7EB3
          03E185DE60DDAB401A84D721FBED440F3CD4E14E47E39F45443B16706E9AC6E2
          D7AF7F83429C41549415E45F0664BE51D68D5850E263A4343B0304180006EDC3
          883B7EF2AD0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_02'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001764944415478DA
          A4523D4B0341107D77B968218226690CD6FE03B131483024580409D82808A228
          5641AC8385298308420A458895858282A001110B913482AD6D2AC17F7077FB31
          EEEE25A7214612F28E8599D999376FE6D6DA281DBFBD7E220E601C7D623E8966
          ADBC37AB6D470899FC382D4E61006C1E9CF86DDB998CC5BE2A17B74980FA2CB7
          3012010BDD62A5F6EE73494288F0542FEF7AFA5C48DA2A1D358908FA389A84A9
          E66757F786707B65C9686142AA5E01DA3EC882E5581D7A0C81EF0BAC2FE760A9
          3B974B4849F0982AB0C81405BED0EA11555F178146EDA66EE65BCD67A0F21591
          2AA0602FC6D7040A2E443781EA85426E314850895215BA9E68294018FF0B4E7B
          C6EB872713C867D346B2CB7970D11AD9F5FF21F01947369D32D99EB28D02A697
          F8F36B3DFE0F815097F5E71713C82CA44C19D75BFF0526A837417C620CBB6B05
          D82DB98DD12866A6136152236277F81DCF6AE7B0FAE8BA5E6290A71CB5899D97
          F7E70C01116118D8181243137C0B30003C44C986BF6CF2B00000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001E04944415478DA
          62640082849AFED3479E31080399BC0C44021B298607F39B0B4C59409CBF7FFF
          495D9F9927C9400248AA9BF40B44830D1014127AD9BD60BD1403C37F22B53332
          B03133FC8673F3BAE79FFBF5E7DFFFBF7FFFC2F1D4659B70F2FFFCFDF73FB9A6
          F7C1FFFFFF212E0081DF40CB67ADDA0A66A7867882DDF2FBEF3FA05D1000E333
          FC6764606461845B0E37E0D7AFBF0CB1FEEE0C8C40B91F7FFE31FCFBF79FE1E7
          6FA006C6FF604D10FE5F90EB19588110C3001098BF6E3BD87F91BE2E0C40F540
          83801AFE43C205CC071900043F18FE621A00B48B21D0DD19A200A8F01F50E38F
          9F7FA12E60808BA303963507CE371D3E7F4DECF8953B0CDFBEFF824B7CFCF197
          E1DEDD9B0C5272CA0CB080F8F10B8B01872FDDF930A1204A1A5D22D45E8F61E2
          CA5D0CC2520A70B19F7FB0180063ACD8B08D61F5969D0C576FDE01F36F1CDE0A
          0EB8DF7F116903998D61808DB931C3CDBB0F18B8B938195EBC7E0B161315E066
          50931101B38F3133C1D9580D58BF6D3783BAB2028391AE26C3E19367C162ABB7
          1F78BAFBC8E9573035A7AFF4C335B232415362FEA4D545C014F5DF3F21FBFFD9
          4B5741CCFF3B0F1C01D3F6891545F81234282532C1385E4E760C5159A50C9B76
          ED6770B3B7263A5331E2B3459893496CDDB4B60A7C2E00083000976AF9CFFEDC
          02200000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_04'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002594944415478DA
          62FCFFFF3F03232323033AF04FAE4D636666EEFCFBF76FF3C6B9CD7DE8F2207D
          20C0886E0050A3343717E74A21015EFD8840379E65EB767E7AF3F6C3F59FBF7E
          47020DBA8FD780C8DCD684BF7FFF4DF573B765F7F7B06566626262F8F7EF1FC3
          86ED877E6DDC71E8E7EFDF7F3280862CC3302020A54E5458887F033B1BAB496E
          4A189BAC94388697EE3E78CA3069CECA6FDFBEFD38FCFDC7CFD80D739A5E83C4
          99A0F2AF3839D8CDEACBD2D8C44485197EFEFE838165A4C5195AAAB2B8848504
          5C41EA6106C30C60F8FDE7DFF7E69EB9EF9EBD7AC3F0EBCF1F0CFCEAED7B86FE
          E94BBF0203F53BB2CBE00664A547F26AA82BF3B4F4CCFD71ECD4A54FFFFEFD67
          80E11367AEFC6AEC9CF55356568A353529941BD9001618E3D7CF5F0C3656466C
          AACA720C2BD66CFB7FE2CCE597E1816EE2AB36EC7EF3F6DD47CEE4F8106E0141
          3E86EFDF7F326075C1B7EF3F80923F18444404197232A2394581C1DAD2338741
          4242942F3335829B978F9BE1DBB7EF0C5F816A701AF0EDC70F30FDE7CF5F0617
          474B5690B8ADB509DB4FA0EB80A10F96FBF1038B0B989999DE9C3977E50BC879
          608340AEF901B109662BC8F01F4083AE5CBBFD19983EDEA1840130F1D89CBF70
          7DD9BDFB4FE49C9D2C448404F819FEFC6581BB0C9880183E7DFEC270E0E0A997
          EFDE7F7C0A4C5C51282E00A6AE9B4041B3F7EF3F4E58BF7EF7A76BD7EF7CFA0E
          752AC8D61BB7EE7F5ABF61373049BF9F0C5207520F33005B5E3003F2978A8A0A
          71BD7AF5564A4C4CF8D99B37EF3F02358603355E269899A0867002A916606608
          FFFBEFDF0220BB09A8F917CEDC4809000830000D226F5EEE89BCE60000000049
          454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_05'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001AF4944415478DA
          9C52BF4B0271147F277A6253410D2E2D612EAD2ED6944314D8D270418D2D414B
          7F40537310D1146DE2D08F49B1100AC26C1029A8065111A5284329CAF3EEFCF9
          FDF6BEE75DE8DD35D4071EEF7D3FF77DEF7DEE7D1F0706A41E4A54949BF0F125
          03A554E5C6DD23204A0D78FF9480681C43A1580213E237196A442CF148ADB0B5
          13A69CB140289AA2945060D6251DEC084008510D25F5BC86A7E7975EE09D5988
          E8E4E169C2D4692F14B754B0B97D406D98BC827941CD43B3D9EAFDCA651AEEEE
          F36AAC288D014EE75B78D78E5ED810E661FFE85CC038EC74F2EAC7B980EF47EA
          90CB65E2187887036CD9E4D9223BE85E9264930249512C15C8A8CC6E1C62BD2E
          4107071798ED756B5333C7C0CE4A433117A889223471D0C9AB348C8D0E8377CA
          03626D9053078FBC2C5B28C864B350C1394E4E6337EC526D2397CBA99CD7EF03
          7D8DD8B95C7E33179025055697D7D52D245D7C7F4C71F23C084B6B600553818B
          E35D0EFE005BD53F2170F93CA08FFC76892D9ABE2746702CF1A4500F124100EA
          F1581628BE5620767DCBC2A8FEDC03604598927F29D0BC5BF36CC8A4CFBA9A91
          3EAF9BFA20DF020C00524F35364B6435890000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_06'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002274944415478DA
          62644003272F3DF8FFF9DB4F86771FBF31FCFFFF1F2C262729C8F0F9EB0F86B7
          1FBE32FC838A81C0DDFB0F1830C0CEA3D7FFA383AD872EFFC7066A7B97FE6744
          3760F1E693FFFFFFFBCF00C27FFFFD01DAC8C0F0EFDF3F30063A094243C1A3C7
          4F210C751BAF4D30C1396B0E61D83469F14EAC2E286C9EF59F09A8391AA8CF17
          4A33FCFCF90BE295BDA719CE5DBC0D667FFFFE03450C26FE0BA896054887E784
          7B324C59B93D1CC85ECACECE0696747736853B958B9313430C04D8585919986E
          1ED9E607E2C0E8AF5FBF61B8E0EBF7EF585DF00DE83216F440FCF2E52BC31F60
          C0393B416CFBFD1F530C0440FCEF3FBE33B0AC3970BE4942909BE1CABD67F008
          3E7DF52E031F3717C3EB97CF184C4D8D193E7FFACCF01318F8470E9C66101511
          8004BC8E2AC3B76F40171CBE74E7C384DC10066C60C9A54B0C0ABF1918AEDFBA
          C5F00A18B6EA56A60C305B40FCE7CF5F20BCB062C33686D55B76325CBD7907CC
          BF71782B28DA19C2825319D8D9D818C28353B05A0237C0C6DC98E1E6DD070CDC
          5C9C0C2F5EBF058B8192CCE96D731819F0002695A7D70D408CF5DB7633A82B2B
          3084FAB8331868A9832561CE052534583AC13040F4D32B231063EF91130C6A40
          037CDD1C195CEC2CC1929F3FBC65983C79F27F6F5B635F509020A75838985CDE
          B408942C672E5AF91FA8E0FFC69DFBE049B5AE2AF60A2117305A45E657FFFBFB
          9B15C26304E62EC6FF201A8445B8D98437CFE9A98506C75F280DC3601F020418
          001FE25B7E25F1AD970000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_07'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000844944415478DA
          62FCFFFF3F0325800544C805141993A3F9D186BEB32C50F619321DC0C808F202
          2323239827EB5F88D7258F37F69F45E683F4B2A0A921E41246AC6180044C08D9
          4AC88033C4D84AD005846C25C6058C444703722C101313C861832D164849138C
          D8BC803526F0C50CB92E6044494DC8FE27360C08A544D2628112001060009FC4
          354689BC45A90000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_08'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001004944415478DA
          62FCFFFF3F0325800544C805141993A3F9F1C6FEB32C50F619321DC0C808F202
          2323239827EB5F684CC846AC5E4002845CC248C80013626C25D5058CA4186082
          6EAB8D8D0D3F905201E247478E1C798D6E00131617A0BBE218546C02361730A1
          F917E45C13586C006DE704525A40DC0DC4ADC478011E0E40CDAA40CA1B2A0632
          A80828160BA4B581F83B10DF00E2B9D80C0085C35F20364372760E10E742C362
          3E10BF03E2A7405CC984C305078001B60C480B40C57C80FC29403A05880DA02E
          8B038A7D473100EA7713605808E088356628FD97503AC0887B6880AE00625014
          7F01B181627A2C5862011798000D034368205E05E2690001060004834C8619DE
          D6340000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_09'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000C24944415478DA
          62FCFFFF3F0325800544C805141993A3F9F1C6FEB32C50F619321DC0C808F202
          2323239827EB5F684CC846AC5E4002845CC248C80013626C25D5058CA4186042
          8CADC880098B0B708683EE14860940CC8CE23CE45820101377045CFB3F00E983
          401C7A3987E135362FE08B091F286D0F5203744910D090B3D80C30C1E50224B6
          1C101F061A92428E0B60E02F107F413100EA7F135CB120E00A673E021908F4C2
          655CE9005FDC830211E4FF77601E29D919E8E76940CC862C061060001ED540E0
          9440AB210000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'edbmdimages_10'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000CE4944415478DA
          62FCFFFF3F0325800544C805141993A3F9F1C6FEB32C50F619321DC0C808F202
          2323239827EB5F684CC846AC5E4002845CC248C80013626C25D5058CA4186042
          C85649BDC5338154F7F34BB177407C262C2E20140E69402C01771E722C101113
          97FFDC37FA09A46D812E3882CD0B8462429E5018608B89CBC030F9851406A821
          8CC50BE899431EE8EC4C205D81C5B26EAC29115B2C006D5E0AA4A29084360171
          10AE74802DEE13A1A1EF04F21610470003F22F8617F001A02BF881D40A204E01
          6A7E0A12030830007B564F12F6A885280000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'tbaccntuimd_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000E34944415478DA
          62FCFFFF3F032580898142C00263A4ADB9D5F3FBD72F6510FBEF9F3F60B1DFBF
          7F83E927B7AF721EEE48F0C06BC0FEEDDBC48194AC988424839C92D2E347F7EE
          C9BE7978931F24C7F8FE11904CC0EE045018A0E38839A7D6AB4537DEC626878E
          5970060EE37F499BB4A6E33CA2D2DFC5D48D3FE2F2124E03FEFCFEC374E7F143
          11B19F8CBF0595F43EE2F2125603585959199858589FBE38B04815C48F9C7B7A
          3D48F3CD2575AA548F46AC0630B3B050680033338697484D89AF80F80A492911
          19CC09D74C6708AF23CA4B440522BA97C889059C5E62A4343B0304180055437E
          52C31BAE170000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'tbaccntuimd_02'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000018A4944415478DA
          62FCFFFF3F032580116440C1E4353540B629B19A5EDCBF7E65455F6D3588CD02
          15339D901BE2F7F7DF3F4CD5581C1896572B0663B3204BACB9F486E1C5C7EF0C
          FFFEFE05F3FF02E9678FEE3374C63BE3740D8A01274F9D6578FF9B99415B4D91
          E1EAADFB0C4FEEDD6460FCF89C81815803FAD23CC174EF9E3B0C4FEFDD62D8DD
          9B4B303CE0068002131E2340FACBAFBF0C691D7319B4CC1D21DEF903F14E4F92
          2B7603FEFC836010F80734E7FB8F9F0CF71E7C645035FACB70EDF67D86C7776F
          31307E7AC6F02F118701BF819A7EFE6584709859188478B91856B79532CC3C74
          87E11150F39AB66C884B7079E1CFEFBF0C3F7E41A47939D918142485197EFCFC
          058C917F60AF81D820C0C9CC8ADB053FFE42C2C05D43188803C17C9010481426
          F7F73F0E17FCFAFD87E1EBF75F58D22A13C33F60A0C0E4FE72B160370014CA3F
          7F611AC0CDC1C2202326C8F00B2AF7EF2F0776038405B819F878B9199818510D
          5091166148B65684F33999B178E1EB972FFB3D532B89CE81C29C4CB750722325
          0020C000D554B45340D5C7A80000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'tbaccntuimd_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001454944415478DA
          62FCFFFF3F032580898142C082CCA99EB5B10948D56253F8F7DF1F867F7FFF33
          7CF8F8B1795655721D56037EFDFC519B1EEE0354FC97E1CFDF7F0C7FFF826808
          FB0F943D65D13A9005B80CF8C970E4F25D865FBF7E33FCFEFD87E1E7EFDF10F6
          1F201B48FFFAF39B818D8505B7177EFEFCB964E7EE3D3178FDCCC2D28CCC671C
          5CB1800DB8CC947206524E20F67F602CFCFBCD08C4FF19FEFE6460F8F7F3FF3E
          16222C71DA93FE4C078BB884799904115EC01244072E6F1706A78DEF448401C8
          B920B0E2D06C6910DDB9A65CAD7C51A23588FDE7FB7FC261F0F71784EEDF5C6B
          BCF3FC7AE96B4FCE49243A155E02CBFD20C6053F2034481348B38992ED930CCF
          F20760B95F44B8E0CF0FB01724809A80ACFF8F323D2B5E80F8402CF0EF171109
          C9AC58C2F9DF4F06A73F40DBFE038DF8FB1B18AE408DC0EC0202FB00020C0010
          1DACDDDAA0EDDF0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'tbaccntuimd_04'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002474944415478DA
          62FCFFFF3F032580898142C082CCA99EB5B10948D56253F8F7DF1F867F7FFF33
          7CF8F8B1795655721D56037EFDFC519B1EEE0354FC97E1CFDF7F0CFC9C2C0C42
          DC1C0CFF4110ECD3FF0CAFDE7E005980CB809F0C472EDF65F8F5EB37C3EFDF7F
          18426CB5198EDFDFC770F5E571B0FCBF7FFF19FEFEFEC7B069F9ECD6BF3F81FC
          9FFFF7A118F0F3E7CF253B77EF8981F11585581876DE59CD9064590CE6F37072
          30B03033FEF4578F33616166110B9F64C1C0882B165C664A390329A73F3FFEB9
          3A28FB8BFCFFCBC8F01F1806C0A060E0FCCFF72BDA3AFB7B48A7C536165CA1FB
          E5D56FBF2571C7351EBFBB2B262BA8CCF3FEEB1B36416E915F9F7F7CE0499DE9
          CD0E34E0C29FEFFF195870D9CCC1C76CB9E472B7C8CF1FBF04F8D805D95EBF7F
          C5C9CB24FCD75ED38B594544FB0D38667E3030607381D39EF4673A379E5D1496
          1752E379F6EE11A7A4A02CC3BB2FAFFF07B499317B1946FC911554F90C0ED45F
          585CC0000D1258D03C797B9FEDECDD63FFB5E4F419D5C5F5FF89F34BFE666560
          E7626262FAFFEF179694F8EF3744E7A1CB3B7940F4F1EBFBFEF46EAA64E466E3
          FD27C221F9F9D5BB175FD6ED59F1C821C9600B507A1F860BFEFE82D0F3F6F689
          5DBC7FFAF7D97B8759E2ECF37E4B8BC8FFE666E3FB212122F595F11FE3EE7B3B
          DE5563CD0BFF7E40E848EBCC4740CDAC0672965F1C34BD1EBF7CFBEC33B2CD58
          532208FCF901F682444140FD0B767676864CCF8A173029649B6100232199154B
          38FFFB094C40C010FEFF03947481010AF416307B80C0BEEB1B5EED45560F1060
          004C1C0BCFF09D2D550000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'commonattrubuteimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000B54944415478DA
          62FCFFFF3F03258089814240B1012CC89C2D276FFEFFFDF71F5E0DE7EE3C6368
          8E7366C46AC0FBAF3F18629DF4F11AB0F6C855DC2E00811FBFFE90EF051000C5
          C9BF7FD8BDC1C4C444D80090E6EF385CC1CFC54ED8802FDF7F81C3021B606561
          266CC067A001AF3E7CC16A80081F1761037EFCFE03760536F0F3F71FC206284B
          0A31C88AF26335800DE8851F5F3FE236E0D693370C65737782D98C580CF8F4E1
          1DC3AFEF5F51C418877E6602083000AA6845270567AAD60000000049454E44AE
          426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'commonattrubuteimages_02'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001104944415478DA
          62FCFFFF3F03258089814240B1012CC89C2D276FFEFFFDF71F5E0DE7EE3C6368
          8E7366C46AC0FBAF3F18629DF4F11AB0F6C855FC5EF8F1EB0F563C71E371304D
          300C40713263DB2986BFFFFEC13188BFF9C44D86FF84C20004FE01358014FFFE
          F38F21D1CD88A174CE4E8623571F324403BDC6C2C448D8802FDF7F3198A84933
          CCDA7E86E1C2BD170CA76E3E6170D0536488B0D765F886C50B18067C061AE06D
          A6C6F0ECED678613371E335868C832C43AEB33BCFAF08541848F8BB0013F7EFF
          01BB22D24197E1CFDFBF601AC407819FBF897081B2A41083AC283F986DAD2D8F
          22C7C6C2CCF0E3EB47DC06DC7AF286A16CEE4E309B114B887FFAF08EE1D7F7AF
          28628C433F33010418006D648688AC52A4D00000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'commonattrubuteimages_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000012A4944415478DA
          62FCFFFF3F03258089814240B1012CC89C2D276FFEFFFDF71F5E0DE7EE3C6368
          8E7366C46AC0FBAF3F18629DF4F11AB0F6C855DC2E00811FBFFE90EF051000C5
          C9BF7FD8BDC1C4C444D80090E6EF385CC1CFC54ED8802FDF7F81C3021B606561
          66305291C46FC067A001AF3E7CC1D0AC202EC0F0FBCF5F062B2D390281F8FB0F
          D815C8C050599281F5EB3586D757B63030BFB8CA70EAECAF9D7FFFFC9F6819BF
          751B8601CA92420CB2A2FC182E7879692B032FF36D06057D5E86A70FBFBADDBE
          FA09248C6AC0AD276F18CAE6EE04B361294547419CC144558AE1E7D34B0C7286
          FC0C9F3EFC66F8F79391E1C3BB5FB6185E404E61E8E0D4C93F3B9F006D06697E
          F2F01BC38F6FFF0F630D035CE01FD0CF2067836C0669FEFDF3FF44B04B29CDCE
          0001060099CC83038275120B0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'commonattrubuteimages_04'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000017C4944415478DA
          62FCFFFF3F03258089814240B1012CC89C2D276FFEFFFDF71F5E0DE7EE3C6368
          8E7366C46AC0FBAF3F18629DF4F11AB0F6C855FC5EF8F1EB0F563C71E371304D
          300C40713263DB2986BFFFFEC13188BFF9C44D86FF84C20004FE01358014FFFE
          F38F21D1CD88A174CE4E8623571F324403BDC6C2C448D8802FDF7F3198A84933
          CCDA7E86E1C2BD170CA76E3E6170D0536488B0D765F806F482918A247E033E03
          0DF036536378F6F633C3891B8F192C346419629DF519B8D85980AEFACB60A525
          87DF801FBFFF805D11E9A0CBF0E7EF5F30AD2A25CCC0FAF51AC3EB2B5B18985F
          5C653875F6D7CEBF7FFE4FB48CDFBA0DC3006549210659517E30DB5A5B1E2EFE
          F2D256065EE6DB0C0AFABC0C4F1F7E75BB7DF5134818D5805B4FDE3094CDDD09
          66C3824B47419CC144558AE1E7D34B0C7286FC0C9F3EFC66F8F79391E1C3BB5F
          B6185E404E61E8E0D4C93F3B9F006D06697EF2F01BC38F6FFF0F630D035CE01F
          D0CF2067836C0669FEFDF3FF44B04B29CDCE000106006541BE644AEBF9B80000
          000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'commonattrubuteimages_05'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000764944415478DA
          62FCFFFF3F032580898142300C0C608131B63E66F87FF93D719A74051918BC65
          1918510C006906491003B69CB90B344019D5050FEFDE6560505626CA803B376E
          004934032082C48107776E638601B22021C0FAFA3AA6010A2AAA0C4F8F6F22CA
          00694B3F389B713429536E0040800100A9F322876BDF7F760000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'commonattrubuteimages_07'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000018D4944415478DA
          62FCFFFF3F032580898142C00222B63E66F87FF93D711A74051918BC65191851
          0C006906491003B69CB90B344019CE67048541E6FABBFFE595118296FF6E33B0
          B030C3F9DF7FFE463164F1D2E50C0BFB1A18E12EB873E3068A0211D6D70C293E
          560C1F8C64C0FCEB53578369333373303D7DDE6FD4307870E7368A01C650FFDC
          98B686C1C2C282C19250202AA8A8323C3DBE0929A4E2197EFFFDCFF0F7DF7F86
          EFBFFFA16860636664F8F71FCD80DD95DE40FF78C305E76C39F6FF3750D1A183
          07C08A4D85A681C54FBFCB62600646FCEF7F8CA80660037F7EFF65B0B4B16750
          D1316278CF30072CA622C5C020C8C5CAF06FE156C206805CF00F18433FFEA2A6
          54342E6E037EFDFEC370F2E821868F6F5F31FCD8D50216130E9FC0F084958538
          03FEFEF9CBE016910A66BFE6EF671053D402B395C4F8800981082F080B7033F0
          F172333001C36BD78AA30C361C6C0C765758C172138D545053223A7048AA8408
          323232B0B17330B0B1B232FCF9FB0FE8ADDF4097FD6610626760583FAD9511A7
          01A40080000300F9E088C2D05700A00000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'defcommand'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          61000000A54944415438CB63601805B401FFFFFF6728C830323B7EA077F691BD
          8DB37292648D41624483EC4425B35F5F8FFEFCFBFBF63F20FEFFFDE3B6EF19D1
          7C86441BB06DB1EEEC6FCFEAFF7F7DDAFC0F88FF7F7BD6F07FF742F969441BB0
          6112FFAC57870CFEBD3A6C05C62F0FEAFDDB3D5D8878031203798D5F1C8EFFFE
          EA48FA3F20FEFFF270DCF7C24801039202313D4CDE68FFD2E4E90797254D2F8A
          9133242910470169000038565AB4D78B84950000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'extreportcfgdoc'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          61000000097048597300000B1300000B1301009A9C18000001604944415438CB
          8DD24D4B02411CC771DF59AFA1EE1D824E5DBA4AD4A54348829065855111D621
          C4C287A2C04844CBC2C8881E08DA0E4A1199BAF9B0EDEECCFE9A59B29A765C5C
          F8EE5CFEFBD919763D003CAB7B27A1F95826236B2D5DB2D7C0C6EE209FFD9F7D
          E3036C05A5545AF1A18CC7CA2B648800EC176E104EE41C4D470E104A9DC1B775
          C8115F4F800FF38BB0B73A22149B4745642FEF859D08C04A326F038649A4C50A
          B738BF7B12901FC0B22C849339572075A5D808DFC978301A1700935A58FE3E42
          3FD7B077F65400346261319E03DF49AFAFD18DCF0C8D4D88C0874610DCC9C260
          50C7A0AE112A011A0C986340DBB450D3DCD36540BD6D2010CDA0DE3651AE69AE
          7574EA04AACD4FF8B78F516580F2AEB9D69201EC68584AE4F1DCD0705D515D53
          355304A61622A174A1A48CCEACBF94AB2D14953747177FAAB77411E8C67F8E0E
          01541D681A620DFD379DCD48818111EF242BDE4F0CF0F367BE000D9F51BF14BB
          C1B30000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002174944415478DA
          8C53CF4F1341147E5B8A6030C542AA5D5B3456287AF4684C43F060F430130EFE
          011A12D478D1935C4D8C0917025713A21EFA0738939878F1E4C5031C1A83DA42
          A584A64B7FD8E5C766B7C5EDF8666CCDB614E4252F6FE6BD9DEF7DFBBD194D08
          015E9B4B7E14B5DF2ED875172CE700AA564DB989FE79FEA1061DE6EF4C64B70A
          70F9EE7D3885EB41F40BCDFCD2E20274B3430096E8E33F336922009B213B4DF6
          14AA313F11C046712FB99DF92097C493968793DD00B44E0D5A3636BD2001183A
          CD2C3DEDDAFD4880FC9777CCCBA05830F8F5A9E7B41B80AF0D4DD3C6D16F356A
          3B441F89821E8D808CE2609FC8BCAC1F0B801641469F1A5611DC620ADCD25715
          1B761564FEEDF4ED7BFF155151CEAD73F47611DF3C23378281971853DE89B469
          2069CA4EAD7D7D7106408D11C85AC66057E261584F1760744C977A70395D1F1C
          6376A50A76D924ABCB6B2C32D40F4EC584C8D069907B5C13A76C8236F16096EC
          570C7666387C0800F3F0A4771BEE0CD79A14151B659B855DB8A807A83F7836C0
          D169692BC742D14BFF3E28E57330128BD3447605D2AB8E62D03A9FFFE520937E
          6A1965AE3D7EF5FA2F32CE3DFBE31B8BC5AF4136FD1D62E357295E612E8F3C7A
          FF8218568385077C8011C2033DA881E012CDAF9F0F29581484EBE74234B5B2CC
          6E2612D4ABB45D31393E2CBAB9D7C342BD2EB59D664D02040703DEDFE6139393
          B4F3E1D4772D55932075BBBDF647800100791DF0A951E73F140000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001DF4944415478DA
          62FCFFFF3F03258005C62898BCA60648996253C42F2CCAF0F1ED6B06797E9605
          857101EB5124412E00E1FC49AB3702E9FF7FFEFEC5C47FFEFE5FB2F7CCFF43E7
          6FFCEF5BB83E10A6078499D06DFBF7EF3F26062AE4E4E662E0E0606730D1D35C
          D7BF68432086176020A1771D43A68F19C38F177718F6EEDDCBA0A1AEC110151D
          CDC0CEC9C1F0F8E37786EF5FBF313CFCF82701A8743D562F1003622BBA8EC3F4
          B1A087474CF75A869A083B86E777AE303C79F28441464686414141818171C51C
          86CFA78F31E8AEDBCFF0F5D307B81E9430F8F38F81614E61108382A408C3C3C7
          4F18422363C0B4A4AC028344690B83EAAA7DC0F040F5328A01BF819289C030B8
          F2E82D83A8A40CC392254BC1F4ED7B0F191E76D4325C0F7166F88B2B1D80C0AF
          5F7F197AD27DC06C49212B0603E4549153CBC003A4FEFEF98FDB0090E905D337
          31C4B81A33FC7A7997E1C4A1FD60710FFF1006B19DAB187E9D3FC120B8763F1E
          17FCFEC3D010EF01E148893268195A2024D3ABC18AFFFEFB87C7057FFE3254CF
          DDCA106CABC7F0F7F55D86F3278E80C5ED3DFC18A4F7AE65F877E934C3BF35FB
          701B202CC0CDB0B82292818911C4D366080DF483180CF2B6830D98CDC98C2310
          2F9C3FCBE0997A76133139509893E9168CCD486976060830001A96145AFD4E66
          7E0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_07'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000D74944415478DA
          62FCFFFF3F03258089814240B1012C20A27DFDC97B776FDF542445A38AA2FCA3
          8A507B79B0018FAE9F570C8ECB4051B069D10C063FA818321B495E8E81C11EE2
          826F3F7E315CBEFB144501B2182E79B8171858D818CE1CDE8BA2E0F3DB8F0CDF
          A162C86C18E004EA41180004467E71280AAE6F98C1A00915436623CBC30DF8F9
          E327C3C3BB775114208BE192871BC0CEC1CEF0F6EA51542F7CFCC4F00F2A86CC
          867B01A8877A5E60FBFF87642F80F4C00DF8CCAF78FFEBE9DD6809899D012886
          850D0142A2D28F4034E3D0CF4C00010600595089D18E2E4BD60000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_09'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001164944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FFFBF7EFFF
          DFBF7F89C220B54159557B417A612E60F8F38F81E1C3BBB744D92E242C821A06
          20F01B1819CB972FC7AAC1D5D59541535313CEFFF5EF3FA601BF7EFD65888C8C
          C469EBEBD7AFE16C1E7E614C03408058177CFFF50FD3807F5085B8C0F5EBD7E1
          6C7965754C0340BEDABD7B375C222F2F0F4533B20BBE6273C1AFDF7F505C806C
          233A5F5A5115D380BF7FFEE2750108C05CF1E1EB2F4C038405B8191232F21898
          1821FC2FBF11B6CBAA6832FC05FAF123541F17071BAA018F1F3DBCE6995A3983
          94A4CCC9C2F8084433529A9D01020C0082AA98488695669C0000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_13'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001504944415478DA
          62FCFFFF3F03258089814280D7004646C6B4817581362B830041134081880D9F
          D6175F724481F3CF25478DDDFF77CD60C6A50E43E0BCA954D57165EEAFB7236D
          FE7F5FD2F4FF76B4FDFF136AFCBFAEBAEBF462338011168DD79D5503BF3E7F33
          8B57495644CACE9281574301E841765050327CBE7B8FE1D9DEA30C5F1E3CFEC8
          2B2791A1B6E5F20A78400B303164B70AB3041A70B138AAA727330968A932307C
          7CC3F0FFE74F06461646882A362E06067E11860F576F31DC9C3EEBDFA5EF7F0F
          54BEF9BDEEFDDFFF53E12E382AC562C9CCCEBA9C5F55554A222C869547909BE1
          DFBDAB0C4C4A3A0C5F3E7D6378B17CD18F8FB76E3DFBFDFD6794EDCBFF277106
          E27159F6EC6332AC9FAF7B99FDFBB468CA7F20FD1B68F87B204E232A1061F884
          2CDBFC8392CCBF8E48324FC6A506AF01200C4C0765F8E441186F42BAFA9BE103
          CD933223A5D91920C0006C5F279564FA89670000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_14'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001874944415478DA
          62FCFFFF3F032580055DE0417E94F9CFD7AF2ABE3F7B66F3E3E95321901887B4
          F43B4E29A923ECA2621D0A13979D4456CF88EC827B1941A95F6EDF6912737595
          E0515561E05151058B7FB9739B0128CEF06AF7EE1740F13AA519EB66631800B2
          F9D3952B1B14D3D224783534B03AF7F38D1B0CF767CD7AC1A7A313007309134C
          12E46C90CDB8348300480EA406A41626063700E46790B30901901A905A8C4004
          05188FA20203C3DFDF685A188118114E2035B0C0458D055058FCFE495CDC2105
          3C523402057FFD40B594F13F84839154B018C02121FEEECBCD1B223C4AF250CD
          D8F57CB9FF9081435CFC1D4620728A0A1FF972F70EC415BF7F406830FE8E1003
          62901A4E31E1231806B00BF075BC3A7CFCC5E79BB71818FEFC846A00D1BFA0F4
          4F86CFB76E3180D480D4624F8951B6A95F1E3D6D12B33096E09193660061B0B3
          1F3D05E35727CEBE008AD5292D3B3C1BAB01E01499E064FEF3DDC78AEFAFDFD8
          FC78F9069217C445DE718A8A1C6117E2EF5058B00F775E20073031500800020C
          009207C47CB24D42BD0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_15'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001D74944415478DA
          A493BF4AC35014C64FDA425BB5D5B4A98D92580C3A38752A15ECE003084A179D
          14C4BE8383765087BE83837F265D44A10FE05007E920A220540984041ADB9250
          458C5A13EFBD6D4A3571D17BB937E1DCF3FDCE979384B22C0BFE337C3F03DBC5
          E57443D7D6D5462D53AD3F46706C2416D75866B8C4D091C2E6DCE1656F3ED5EB
          60E36429272AD2D66C6A8615F804085C82C4510C445982F3F2858A62F99DECD1
          AE03802BDF89F7A72BF38BEC644270B57B2F89707076AC4E09930BB6138F7D88
          6DE3CA4EB1D9BDE2339C8373EDD32E003FB3C0F324D182CFCE6AA16575EEF134
          611CE5A88DC78CA389D5BA1AE13916DEE19508A89EDAE459C90418E3E26037F7
          1B00F30D6822915D1D3B69C7B1510FD9BD646FC75C002FA023E93B8198C8BED9
          F140211145205E347DEE002616D62A4A8519E64248FAD1017C92548AC8DBE29A
          F20CD15848730068A6AF24CAF24210F5E1A30B3049B536C04796285761880996
          1C807EDA57B82A8BD3FDA3164B270248DE227DC0ADB4019A64C075595279215A
          70FD12574F92395579DA9A48D12CCD0780E6FCC481AEBC812E1BF050D6D53817
          CAEF676F765D0178AC1593E927FD75BDD93032CDBA415E5738E6D70699406980
          F617F6E66E7FFF17FE323CF0CFF125C000DA94DED5656A2D8D0000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_16'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002434944415478DA
          62FCFFFF3F033238B7C671D3EBDB4F7C19D080A8AACC66A390FD7EE8E28C2003
          346CBD0D816CA39FDFBE984F0A7A9DEA5B3D1DC8E54752F68C61736B0943DE3A
          D1D9EC5C3C278102176F1CDE7A0624C302556134ADB7DD58554A28F5F83C9025
          8780581CC980970CDFFF72301CDAB034F5D6D3B77FB24BAB7880822806683B99
          E9653E7AF498414C3F8161E98C170CCF1E6C826B9752D06390068A835C6B67A2
          9309145A00936341F68F80A02083A6652483D28F1FE85E6560E7E060E0E4E4C4
          104731808F97078C4901700340CEBB78F506C38D3BF7712AD6505164D0D7D6C0
          6EC09F7F0C0CEAEAEA608CD336264660BC3132EC5A31C717C380DFC0E470EEEA
          2D86DB77EE61D52C2725CE60A8A3CEB0EFC172864D9716F13EFB7AD7614FFAB3
          030817FCFECBA0ACA808C69CAC4C0C025CAC70CDBFFFFE6760656664587B7936
          C38CE3F50CD636BA6C37E6FFEB014A99A0B8E0E2F55B0C77EFDF670873B763D8
          F76C3F43DBDE6C0635513D0667D5600639011586E9C7C09A19AE5F7E04D2628C
          E2855FBFFF30C8C8CA83310F2F37C39A0333190C0DD51844453918A6EFAA07AB
          81697E70E63D039F2A93238A017FFFFC65F8F9EB1798FDE91B2B43BA451D43F1
          E610064D2D7906635335B0384CF3ECA2750CFA52560740624C50FD57AFDEBE3B
          5D555A84415D4684818F9B13A480A1D7770DC3F56B0F191E3F7DC570FFD64BB8
          669EDFB257D1339336906D014AD230091B332391A2F404EBFB1FAFB0CFDDDD25
          F2E4DD5DF6F290FEE72632F6AFE62E5B5BD75496BB096E00250020C000AEB2D3
          F662E2177D0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_17'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000A24944415478DA
          62FCFFFF3F032580898142409401F6A5F33751EA025F5C866018A09E3CA90946
          C3D86F3F7D65F8F0E5BBAF7EE6B44DC4B8C0028906B37FFFF9F7E3C7AF3F5F80
          980368E80472BCC00AC42C402C05C45AC886106B0023103303311F104B90E302
          5062F90BC49F8078DFCDB979053009162C8A4FA0D1E060801AB0075933D869C4
          A444A09FBF00A9DB40CD86E4A603AC9A8976C1C0E7059A1A0010600026423DE8
          12CE2A700000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_19'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002224944415478DA
          62FCFFFF3F033238B7C671D3EBDB4F7C19D080A8AACC66A390FD7EE8E28C2003
          346CBD0D816CA39FDFBE984F0A7A9DEA5B3D1DC8E54752F68C61736B0943DE3A
          D1D9EC5C3C278102176F1CDE7A0624C302556134ADB7DD58554A28F5F83C9025
          8780581CC980970CDFFF72301CDAB034F5D6D3B77FB24BAB7880822806683B99
          E9653E7AF498414C3F8161E98C170CCF1E6C826B9752D06390068A835C6B67A2
          9309145A00936341F68F80A02083A6652483D28F1FE85E6560E7E060E0E4E4C4
          104731808F97078C4901700340CEBB78F506C38D3BF71994E46519A4C4453114
          F37073812D5839A3D719C3803FFF1818D4D5D5C19895898161D6EAED0CABCFBF
          64F8FAE337368B6519180EFE17F8FE6422DC80DFC0E470EEEA2D86DB77EE3178
          395830DCB8FF8441E4EF7B0611562CDA4149879181E1E3770673840B7EFF6550
          56540463012E16061B0B1386304763867FFFFE61F53B131313437076F5371417
          5CBC7E8BE1EEFDFBC0A8D2852BFCF9E72FD046460C2770B233A186C1AFDF7F18
          6464E5C158929F9DE1FA937760F165EBB733F87BBA326CDCBE1BAE5D554981C1
          CE540FD580BF409B7EFEFA0561FF4578DCD9C991E1CBCF3F601A5F345EBD7AFB
          EE744B23BD4C26A06BD99810196CFDB65D0C6ECE8E0CBBF6EE838B29CA2B3028
          3898A0187022BFB41244F78388F2EC14FD6FACFC20157CC991C10C204353A342
          E006FCFD8F961BB181E0D2DEDCB76FDF68E14B859C2C8C8F00020C00E882B9B3
          B7FA61410000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_23'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001C34944415478DA
          62FCFFFF3F03258005C62898BCA60648996253C42F2CCAF0F1ED6B06797E9605
          857101EB5124412E00E1FC49AB3702E9FF7FFEFEC5C47FFEFE5FB2F7CCFF43E7
          6FFCEF5BB83E10A6078499D06DFBF7EF3F26062AE4E4E662E0E0606730D1D35C
          D7BF6843208617B0812B5B7A18C42425185EBEF9C0C02EEBC7F0F8E37786EF5F
          BF313CFCF82701280DF60A860B585998E118A45952461A4CEBFF39C5C071A092
          21DAC998E1DDB3FB62585D00F3D7B469D318D8D8163078EBE732FC7D7D99411C
          6A8D283F2758FEEBA70F0C580DF8F30F82412036F114C38DADDD0CCF5E5C4028
          96300086078E680401C7C84C06161E41307B4D7A35985E3F6D19985E3A672A43
          B44736C35F342FA384C196055318FE7C79CF20C8CF07A697F4D6337CFDFE138C
          6D4EAD03D37FFFFCC76D00C8F4C5332630BC7EFA104CFF06AA05E137F95E0C8A
          8A2260FA1F3E17FCFAFD0768CB2F8619FD5D601A843F167831887332327C7BF1
          164CDF4E71C1E3823F7F197EFEFA8582FF36AE6278F91D18F22FDE806985E93B
          7007A2B00037031F2F37031323AA33FFCED9C7703FC5894111487332E388850B
          E7CF9EF64C3D8B27DB9933306454422CE264BA051366A4343B030418002065F9
          085B79559B0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_24'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001834944415478DA
          62FCFFFF3F033238B7C671D3EBDB4F7C19D080A8AACC66A390FD7EE8E28C2003
          346CBD0D816CA39FDFBE984F0A7A9DEA5B3D1DC8E54752F68C61736B0943DE3A
          D1D9EC5C3C278102176F1CDE7A0624C302556134ADB7DD58554A28F5F83C9025
          8780581CC980970CDFFF72301CDAB034F5D6D3B77FB24BAB7880822806683B99
          E9653E7AF498414C3F8161E98C170CCF1E6C826B9752D06390068A835C6B67A2
          9309145A00936341F68F80A02083A6652483D28F1FE85E6560E7E060E0E4E4C4
          104731808F97078C4901700340CEBB78F506C38D3BF7712AD6505164D0D7D6C0
          6EC09F7F0C0CEAEAEA608C0FFCFB8FC305BF8112E7AEDE62B87DE71E4ECDAA2A
          4A0C667AEA385CF0FB2F83B2A22218E375C1DFFFB85D70F1FA2D86BBF751C300
          64A0BA9A2A9CFF1797177EFDFEC320232B0FC6E8E0EBF75F0803B858B01BF0F7
          CF5F869FBF7E616866C4F002075603AE5EBD7D77BAA5915E26139A0E74279F3A
          770994516EA21B7022BFB41244F7139176AE02F16594DC480900083000A1948A
          3EF6FDD13E0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_26'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001074944415478DA
          A4534B0A8330104D42F66EA4B447680FD05BB407E8019A8541846EBC815B1137
          1EA29B1EA65728946C447463D54E444394A8C53E984C98CF2379CCE0FDE9FA42
          08D960E2F94876E8071CCE4CF5D0EED27ACE79099E8CEA6BB0248E63AEC5548F
          3C44CF1686E116633CE86E9A86789EC7E0AA13A81E59801CC729B32C2BF33CAF
          C626E390AF649DC968C748D2347DCF7CDB9E4A50533008823B38A6EB01FA5426
          5DE804318BA288CEBC88B8AEDBEA3255448AA2F81080295903FAD7F504B56559
          B66CE8920834110BE3B0D10912DFF7195A8196A01B123E21D82C085A8FF6AB54
          9F6B6D17942626018F979BE87762B00B5ADD9226AA07FFBB8D58CEF33FF80A30
          00D8CC92CEA21292040000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'images_27'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001284944415478DA
          62D4F44D7BCEC0C02002C46FAE6D9A29C98006B4FCD2F1CAB34025C1744E4ECE
          73064C2082A48E019B016F6036545454887273733331323232FCFFFF9FE1EBD7
          AFFFF66535BF86C9633300AC303B3BFBF9EDDBB77F3E7CF8F0373A068983E441
          EAB0611698410F1E3C7807A2F326AD8639FBCDA4BC50AC9622870B0B3E3F6FD8
          B001EE6CB4F081ABC166003C4CDADADA8498800059F21F1058C494C2D530A1EB
          0639DB498EF5CDE1F96D3C8CA0D0440320034F2CE91602A901452B0BCC3FFB26
          AD7EB3AC299307AAEE0BD0A2BF9F3E7D7ACB4000A0A4833973E6C0FDFCEBD7AF
          EF38F44800F10BB88BA07EFE03A2A74C992209C20C2400166CC99314C0084A0C
          B8E21814583862E11D51E9A0AAAAEA0D0E8B894B07B8C203D995185E20150004
          1800F776AC34DF9147D50000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000964944415478DA
          62FCFFFF3F03258089814240B1012CD804B5FCD2ED81D4012421876B9B661E24
          C50507A09A1841349A61A8001488E858D337ED3F3E3E3286BB809191111E1D50
          9BB1026475702F40051D708407BA9C03BA21208E3D2EAF0031861C483DC4F7FF
          196002580DD149EFFC8F4F330833C25222C85940C04828DED1D53121C506230E
          FFA3C71A8A3AC6A19F1728360020C000A3CDA12B1482650C0000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_02'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000B84944415478DA
          62FCFFFF3F03258089814240B1012CD8041DBB7781FDC5F2E73798FF9F919161
          4F8517235613A06100228A406C107669DBF21F1940F9C86AE1EA99A086804CEF
          4537DCB6FF002E4BE1EA718601D01B0CC6523CB834130E4490FF27849B3044CD
          388821C7C8C808F28203CE400401514188EDAFDF7FC1EA0AA8218C385DB02CC3
          9EA160E519863F2CACD86C272E1D80BCB0BFD40D97178A51BC801C38A078776D
          DFCA80CCC715888C433F2F506C0040800100A795612728937142000000004945
          4E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000B54944415478DA
          62FCFFFF3F0325800944303232FE87627B924D00B90086215C08DBA16BE77F64
          395C9809642BCC05C806B3FCF94DB4170E004D62046172C280059FA463F7AEFF
          50D738ECAEF43E88CB000798F341AE00B161AED95FEAC68864105617B200151F
          4496C4E6157CE1C1448C3FFFB0B03240A3DB1E3DCA7186015045B16BFBD6FF20
          CDA030800A1F80B910EA6DB087890E71E4F021C90B049332390016732C24EA73
          404AB1E07061A44A6EA4040004180048C775B69D1FC8BF0000000049454E44AE
          426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_04'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001F34944415478DA
          A453CF4B545114FEEEEB35CECC1B3014A4C85A248C36E5C24D5AAB36616D9256
          4550BB5AB989C05646B4EB0F68118AAE841005DBBB13C545145848FE28080A6A
          9139CC8C6FE6CDBBF774EE79CEE07B3A1474E0BC7BEF39E77EF73BDF7D571111
          3EAEAD7C72C2726F1B951019E148E3DA9AF2609CECC6C581AB7D36E4D8CFCFEF
          DBF9C2E9103DDD19606703D9DA37F4E4CFF33A2B8E9DCD28D65B40A11BF8F06E
          29DFC01480F654A0C25FDBB03E3339874AF1377348A355AC234BAA01A06C0B6F
          175FD189DDF7F857FB1C9CC4F0DD6702E2DACFA67F862E5D1854507FD9C9D274
          769DC2ECD4040DEF87DC46EEDCD92E2950EA808476A2E2A34A79314CA7591B54
          406105A517CFB17BE79678757E0608CA089617650DCEC304310037C9D27BF4B8
          798DC5FBF790BE39C2002B2DBB6A32409DFF81A0148DF532889D8F83E1D11B7D
          18CD6D5EB76040BC316A5561EFE524C2B575B48DDC1040B202F06D110350E628
          0053170D2C755B9C79705B442B8D3E45EADAE5FD8E4834120646270134330890
          BC469253F7384C328715DA849C4800107150D7E4FCEAD41B84EB5F2499BA7E85
          4FF5E14F2F08037FE235724FFAB83E0150AE69ADD21DAE73DC436E6CFC90D2B9
          B1FE03B4342A554331801F5FB756C7B7F4507B1AC7980AB744A2FAE1C76850F4
          49675CB31A7B0BFF637F04180080EBFE337A2047DF0000000049454E44AE4260
          82}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_05'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001BE4944415478DA
          A493BF4BC34014C7DF35155D4AA7500B5657717011C4C1A5BAB98888A3B3A2E2
          E6D6CDDD4DD4C1FF400411AA8B3F1651717291E2546D2535A5823FA869DABBE7
          BD3B2F49B1A060E0259777F97EDFE7252F0C11E13F479C4E93DBE91D7999A235
          726988D027975C70E6228784CCF93297E43E38A281F9CBF5E7853603129F2C3A
          195A4C6CF6762EC5002E7215185D4B95DAD2D4C2E4569A92CA00852250C76BE1
          677BCC02ECB659D99068028D0C274B4EC7E233C7FDEA553D1E36180A45923124
          CA40705D69F674009944DDCB3EB0A8F8ADD804EF858368EADCC84A2A308F452B
          55AE3EE1C34125A2FBF1535B896B773ED4AE31AB5AA30E858ABED0E07B831E7A
          2F7A50AF6993EEFBB8127B0FB16CE1A07A4E6D508826AA881294E9440F91C9F3
          751D4C65238E80964DC1C0407E5F385BAD803121D1D351EB87F876D705EE690A
          337FFA257AA1FDF0BC0D5D092011131C6068CE9615F4DEDD7E15848F60F5B080
          5A11B4BCBF8FB3F043928080D7213FB29C2A718E69794BC3A04659862B232143
          CA2039386DD35E5E1B616870B3A1677B2C972A5D6DBB19D34ACC920B8BE644F6
          DC6AFFE64412FD17204AD2F283967E2561FFFD9DBF041800FB3B0659F00604D1
          0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_06'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000AE4944415478DA
          62FCFFFF3F03258065CA94294540BA970CBDC53939397D0C93274FFE8F0C76EF
          DEFD1F1D601383EA63600119B567CF1E14A3D1F9B8C4C060D40583C0056003CC
          CCCC18FEFCF903163879F22483B9B9398A227431161616861B376E200C3875EA
          145C92959595E1DCB97328066013437181969616C3AF5FBFC002972F5F66D0D5
          D54551842EC6C6C686E282E275EBD6A1E485870F1F62D88445AC184430529A1B
          991828040001060032EE3869266688DF0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_07'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000FC4944415478DA
          62FCFFFF3F0308ECD9B3A70848F53210002E2E2E8C280220034078F7EEDDFF89
          016D6D6DFF617A4098898142C0822E3065CA1486BD424E0CEBA3B4186257DD64
          E0618528F9F2FB0FC3E230754C1328F5C23070018A019F3E7D62888A8A628802
          B2DFBD7BC730D14594015D1EAF015FBE7C61D8BC7933C3656E6D86121B1986A9
          C79F3370B04062FAC79F7F0C85F6F2F80DF8F7EF1F83939313831390FDFBF76F
          8634131106747982E960DFBE7D0C37B83519B2CC2519E69E79C1C0CECC0C16FF
          F9F72F438EAD226103602E0081641309D252E25FA02D20FCF1E347ACCEE5E6E6
          C66DC0E9D3A78B81B897D4BCC008CBCEE40280000300B74C118F6C81B9080000
          000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_08'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000015A4944415478DA
          62FCFFFF3F032580898142C002222E2CB70A6660142C67E2F865009701BA8C9B
          93B146D96377175E13405E38BDCA6ED3BF9F0FFFFFFB71FFFFBF9F77C0F8EFB7
          ABFF5F5D2AFB7772A1CF22901A5C18EC82FFBF842C19585819FEFF7D8AE2365E
          590746E1774CFE876638BD6666656560656583C95E64637DBBC620F2D80CB001
          3FBF7F036AFECEF0F7F71B54D7FDFBC4A06464CF07C428E2AF1EDE773EB36FE5
          7FA07F2106FCF9F387E1DF9FCF0C7FBE3FC5F0E28F5F5F1860F1C408A57FFD7B
          05349D411F1E88FFFEFE61F8FFE72BC3DFEF2F700414AA09FF7EBE478D85FF20
          0340F8D77706A24CF8F30BD50026262606C6FFCC0C8CFFD8898BFCFFAC682EF8
          07B2810388F9894C3EDFD10CF8FF8F0114A38C2C2244696764FA062418915DF0
          EFE2FB97B79D7F7DFD4494015FBE7C031AC274016E80942CC3B2B3BBA6FE03A6
          2C03E29CC078514EEAD74A3073C0732340800100CEB5AF5401E2AD6E00000000
          49454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_09'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002274944415478DA
          A4534D6B1341187E36BB31A569D6B44993B48DA69654A356AB92864220142F52
          0F22F6E82FF0EE3F118FE2D5837A17F122458A205511C4586DD2D8483E36BBD9
          48369BCCCEF86EA141BBA197CE65765EDE7DDEE763461242E0244B395AD879F3
          F0C540CCDCFBF4E11D1652B1835A289AC6E139ACFA5E9EBFFD78E3B0DF771460
          D0E7F94C2E8BD954042B8573B8BEA262F1CA3CE23119972EF8D1FC5DCB1FCBA0
          D3B6E246A30C5996A0EB5DF4CD2E60EE4270A0A559E89876FC58007755BFEFA3
          F46D1F4C6FC3610C6AE2342A2503CCB43CBDBE7FB43F7DFFEC8190E83B732D89
          E4D920560B716473095C5C5EC0CCAC8AE51B49385CE0F9A3FB4F3C00A47D3D77
          730966A306A35903281CAD6E41D37AD06AB5831EC3B0D1EA30D8A671D723C1D5
          AEB74C9AE020188A82310763A71448B407D4691A40BB22C31E9019033E35D283
          E27619E01C5F3ED7F16BCF44A7D9856632CC2FA5B05336B057FD03BF22411A65
          A25BCCE6A3F85AC9607BEB2DB28545B8D6A789117776B176671D1FB736713913
          45FD47DD0BC0FA368C8681B9C8248C440009954C204B1D472652025250C299E9
          31C426645499F002F4AD2EC2E130443006E6704C84FC1421291A0862C0218722
          348423E057C0B818C5A04F17A54748BD6102C49E18B800027ED8C324BA36F702
          B8EE8F8FFBA14CCE0D131004C0882EF77104A792FF27E14981F3579BAF7FDE4A
          AF1AA8ECB6D1D32D6A14E811ED1EFD70754D47B1344CA23834FFA4CFF9AF0003
          0006E21A97A65552910000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_10'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000CC4944415478DA
          62FCFFFF3F838A59C85106206061FFCE0FA2FFFCE4FC78E7D41A6B0622000B88
          6066FBC9CDC8F88F85811C007201084F9E3CF90A8C4D0AC66AAB967FC605A02C
          428E91F1CEB58D3302707A0106A64C997205AC9E89F523D8757F7FF193ED0562
          BD842FE036C318CECD3B8E32B1FC85BBE6DF1FE68F7B6B3DAC31BC8006F6C2BC
          C4C0A0F2116F34E2001372727274408C1C42E9801020D70B0CC47881560929F3
          2CC4F47FEC9084C4F4F3DAC6E9C684C26019C2C90C3F89CD0A8C206750029818
          280400010600927CD1CBEAA49C030000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_11'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000624944415478DA
          62FCFFFF3F03258089814230880CD0F24BDF848D4D8A0B7C71B019D00D46B680
          095D011007E0D1E08B6E017A18B40171121697F8121588D736CD3C01A44E911B
          0B9BA186B4C0D840100874FE7F7435483403E3684A1C040600041800083A2048
          B4ECF1320000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_12'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000DC4944415478DA
          62FCFFFF3F03258005AB28A309C2D4FF6718F19A007201066630FE8F4DFCB5BD
          F27F641AEC7A9C06201B62CCF01FDD101866514D9EB009E8105FA88336DF9E5B
          E00762A826C53030244FF80F1603F1A1EA8E43D930750C2A49FDFFDB2FFE0763
          101BE6021431A00B4034C8769018B22B188112182E4077D5D2742BDFE899C736
          6373291394B1192AE10BD20CD2802C06D28C2E06B584810926909C5FC000C248
          B61025064B079BE74E9C00771E549218310646584AC4161BC4883121A529B2BC
          819C94319C478C1823A5998989814240B101000106004700F3BB76954F800000
          000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_14'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000574944415478DA
          62FCFFFF3F03258089814230F006B0C018393939E0C09832650A23321F091403
          E5FA487601C84024438BC8F602D4F65E7443C80983DEA11F8DC56419000D3C8C
          A8642142E37F7CE98071E86726800003006D38234D76ADEC8A0000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_15'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000006C4944415478DA
          62FCFFFF3F03258089814230F006B080084646C62820158E457E25308C96E135
          011A881B41343E362E8C2940842664CC42C88F84BC473010A161B01257D81015
          0B6886A0042C0BB1D105D204F40E037AAC30222765A0828D481AFC8931987134
          2F3000041800C49C860062C1978F0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_16'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000003044944415478DA
          54935B48145118C7FF676667662FEEAE6E6BDB62AB5684970CC37A8890B0CB5B
          778BAC20AC08BA422904BD19445414D94B2F053D846F413DF46074230A292209
          0ACA50B3CC72B535F3D2EA5CCE397D33829BC3308773E63BBFEF3BFFFF779894
          12F75FF546FCBA5ECE005DA10FBDF4480821603B1CAA925B53BC415896ED746D
          AEAD1CF7B9F3C5F383BB34456DFD363419118C4110540A090AF2209033303719
          E71C65A9E8B8904A136DBDED0102865EF1B47320FC65282B410087B2DA368743
          C12EC8ADCAE18220DCFB379CC9846BAB8B2BDCBD1E4070138A4F454141D8AD12
          9CB29996E3655B5F154365491C8AC2D8CFCC045E7E484B9D8F518C8959803B89
          8535F8FD2A4C5BC27404A62C15DC9C423A9D862EB3D00CBFFC93159817D51092
          065563CD0538AA4A4AF9E00FA8D0A80ADD1698980006274D8CF3698482802935
          F898066E3324E1E40042D850340D2A0102BAE6CA8064C440717E0279E48B4622
          D0B1645F7A14036326A6C6B374D2E91CC0B2B34812400F1AD03546B6312C4A04
          B17A5F8A45CB554497AA58CEB6B3535B2F48455331EA3072C4CE0148777CEF79
          06C79E84324DB6D9164A761F63A1401E4A65198E5637E1B7DE83F3371BD8999D
          176544B7F1971C9905284CA2FBEB43740D7D7415A16691A8331B5054938FA3EB
          9AB066652DB6B61DC294E1E0C4AD2DECF2FE3B92B976B97B3D11B98560DC42A2
          88A3B048A030058C6507A4333228D9FC51D4DF2BC3D2AA2462CB74984907571E
          9CA5943C07100488C47C58581C40BC508316025E77DFC5E93D2D686D6BA639C3
          DE158DA8A9ACF0203F02FDB8F1F8D2C6FF2AA0A6505404C205044A20128DE26D
          773B7A06DFE1C4A61669BFB171BDE32A36A4B6E420BEAE1575D792CDB300C3E0
          04F0232F9A0FA1FA108E86F079E8257E4D76E3C88EC3D2D769CC81C47F2F79FF
          BC79F09A0770A891F26429B2FD1CE64F1B1133867972010A59027DBD9D30330A
          CED5DF96CA0BBF071968B77072DBF147391714D651996A3C50121B897BF77646
          606F6033F798DA3088D683ED74179EA06AEDAACCF0DF4F1DEEF23F010600BF7E
          5E2309F887000000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_17'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000C54944415478DA
          62FCFFFF3F032580099B2063FAAC228A0C00825EA02146641900D45806653A90
          EB824C982B48360068BB0C905240E21B91EA826420EE43E23B906A400210AF06
          E26262BDC1084B0740E75A00A9E3FF67A63142F9B004620C143B87E42D98B82C
          50FC09B20BB281185BFCBB22734016402D790CE2B320C9F900710CD0863E9CCE
          8504F2631441B017D2665A00F17F101B1983C4A0B808C6479603D1302FCC00E2
          303C6185333019C136A0F91129A0D001285A0F20B31969921B49010001060075
          485DDD8B4ADD630000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_18'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000A04944415478DA
          62FCFFFF3F03258089814240B1012CC89C59B3661501A95E027A8AD3D2D2FAE0
          3C5018C0F0CC9933FFC300840D9102B13F6989FF7F6F24031547E821DA0BDC77
          DF32FCE360C11067448E05A017888A12A017187106E259FF79600C02179CD2C0
          18049E2D5A05C6548F05EA7BA1E77E29188340FA59080681BD3754C198B60909
          94484A14BBE10969A63142C259E3365C0DCE30189A990920C0000A166471E8D1
          A3F50000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_19'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000005D4944415478DA
          62FCFFFF3F03258089814240B1012C8B162D2A02D2BD64EA2F6658B870E17F72
          01482F0BB2718CE9B3508CFF3F338DA01C0B2E0DE800971C0B2E1B9035E19363
          A1C4F6C1E38262605A203B1D300EFDBC40B10100010600D28668585F2331A100
          00000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_20'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000604944415478DA
          62FCFFFF3F030C2C5AB4A80848F532E006C57171717D282220036078E1C285FF
          F101A83C8A1E165C5631A6CF42B568661A5675380DC0A5812803D06D27D905C4
          DA3E385CC0C44021188DC6C1E882626096C69B9DD10500020C00EFD560158028
          F65E0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_21'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001664944415478DA
          62FCFFFF3F03258005445CBF7EBDE8E9D3A7BDB8147DFEFC194415070606F661
          48825CB07BF7EEFFD8C0C78F1FE178DDBA7520210674CC82629A89090383BB0D
          D04957C05C3E10F1F30FC3A7E59B60AEC0EE05B0BA9F3F19984F9C409564F902
          35E81BFE3000990E3260C58A150CA2A2A20CAF5FBF46519496E6C6F0E3C70FEC
          2680FC01F1DF73207E8F849F83FD8E1C1E04C360FDFAFD0CBCBCBC70FF3A3B3B
          33ECD9B3A778F5BD7BBD338D6731A49F4D03C73994CDC07096A1981164CAA245
          8BFEC7C58503A5BE434CFA00C420274B4830A4CF9A05528815BC7AB51DE202B0
          FFBEFE61F8F0FB035CF207D01409200C555262D0F4D6C46AC0A2456F1998408C
          5FBF7E3130BC3EC320F0E3038300900FC2125834BCD8BA95E185BF0CD6682C6E
          5F7E0C232566672BA1F08DD3D218CECE4AC734202727079444FB704435DECCC2
          C4402120C90060866390D8F8047B52C605EEDDBB577CFAF4695C39B51820C000
          4F88ED0171139CC90000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_22'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000018F4944415478DA
          62FCFFFF3F03258005445CBF7EFDFFD3A74F712AFAFCF933980E0C0C64C46A00
          48B38B8B0B86C64F9F3EC1D97BF7EEC5ED0238F07660605053666078F810CCE5
          03113FFF30FC5CB713EE0A9C06806D5BBE0945928FEF1B90E4606027140630D3
          57AC58C1202A2ACAF0FAF56B14456969610C3F7EFCC0EF02906D69697E601B41
          E0EF5F5E06666666B8C25FBF7E61358009C1E46058BFFE38C39E3D6780F47E86
          AF5FBF32A4CF3A5BAC533305252660C0B966CAFF59B3CE163180D2C1C2850B81
          D417207E0FC1CF9FFF0701E3BC89FF61A0ADAD0D44312063903CD80B60FF7D65
          60F8F0FB03D8F41F0C1F182480F0E3977F701BB3B3B3319C0F926741F8EF2B83
          00DC377016C3ACB367ADF6AFBF778C4FEE7DF1CCB434A0B766157D7A24D8EB18
          A8540C9267043965CA9429FFD1FD585959C9A09A3C81414E92C56ADF6BB663E8
          B63B89FE6278F4FC0F0303BABF90B14A52FFFF9967CE144554AFFA9F36736611
          480C4483F82071903C410308C933E1CB69FC3C78A5C1F27855A4695B1783E21B
          9B1C481C240F106000880C148EAD045F630000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'typesimages_23'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000734944415478DA
          BC92DB09C0300845ABCB35BBC4711CA6DB590453FC688DB58F0BC1E0010D8780
          882C8FA203C6E9BD8BD555EF5A670CC72022F24FD99819B4CED831C09AA78918
          5EF49B6D6D3306AF49BC23CE33AC88F30C2BE23CC38A381FF8E527A62446D252
          122369598991B4EF24EE020C00AC84BD234AE183A30000000049454E44AE4260
          82}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_01'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000844944415478DA
          6260201118A7CDDC88CC6762201DF8211B428E01288630A239EF3F89066D62C1
          26787656BA3F9E30F88FAC0EC30B2041F48022DA12642FE03204AFE1E8614084
          4BF01B40B221B86201DD1092D30172C042693F92131292217EC8E28CD804B145
          15BA3AA01C38113241156D2245338A0BB028C29B1249CAAAC400800003006CFF
          4E21E5C64D270000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_03'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000007E4944415478DA
          62FCFFFF3F032580898142C0026398A4CF0239651310B79C9D957E1A5D212E97
          32C224181919198CD3669A0299A7B0194494013080CD20920CC066D0999969FE
          A4B8C003486D27C6052C386CBC0FC466D80213A70140CDB058806B048AE501A9
          8950717F9A04222E2F6CA28A178801C32921910B00020C001F17812A4C067272
          0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_04'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002164944415478DA
          A452CF6F125110FEB625504358B24423D6148B8934E1562FC4C5C6DFD1530FDC
          31A97F85552FF66AEAB9FE883DE0450E4430C1A689BD632FA525C448D0602DE0
          06427765B7B08BACFB9E792B201E8C5F323B933733DF7E3319EEEEF23D539165
          78BD5E9CF49F028320F8EC58910FA9972409AAA6D1F8F1EA238E784728340751
          8CC2E7F3E1C4718126654541EB50866118D0AC867EBF4FBDDA56E176BBB1BEFE
          C2269FA856ABF13799D7D8C9E7F1E163893E7A791EB2A5AAD96C627FFF0BF23B
          796CBFDF86D3E9349F3F7B8A60F06C9C114C6E6DBDDBBD74F94A593B6AC71C93
          4EAAE4D8D414DAAA0AC552D2FF61A2D7EB61FEFC3C9EACADE15C68EEF683FBCB
          2F6D02F22124A278B10C0E315D37307B2600EDA803E99B449B67023348BE4A62
          7AFAF450330197CBE5D2965FA4CB52BEA3FCE933C46814C1D900363636A1EB3A
          4AA512A2E205F0BC0723C83848732412A11BB5C8CC46A311CF66B3893B4B4BA8
          D56AF87A7000DEE3895BCD0956C7904EA74D076B24BED56AE1C6F5AB89ECDB4D
          743A1DEBEF3D189691379263750C954A05132420CCC40441A0B16EE876111961
          303768F40E58612A95B2D90DA387DFB13E3474B158B4EB0A85C22F0504B158CC
          9ECFD00715184304E1709863460F699C828505110F5756B0B7B78B5B37AFFDA1
          80190617381A8F625C2E994C9A637730382793FDB71DFC97027207F412EBF5FA
          62B7DBC5BFC0E572C1EFF7677E0A30002E641E7E3BE51BF10000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_05'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002624944415478DA
          626440038E399DA6B2B2B241FC3C5C56EF3E7E567EFDF6C3DD8F1F3FEDFEFCE9
          C3CE6BABBA4F331002BE35B30F1FBBFDEAFFE5B7FFFFDF78F7EFFFF1475FFF4F
          D971E9FF8495BBEF6053CF625A2CFEFFC7EBFF0CBF3FFE67B8B1F1352310D8DC
          FDC2C4A02FCCC070E4D60B869357EE303C7EFA92615A8A83F2923CF1EB7F7FFD
          BFFEFDF9FFF5BC2A4C8B4EF7BE6464FA7CE75F1C2313C30D567E46BC2E3B75ED
          3EC399492F35819AD7B2093256812C0501264E49C6C05FEFFFB77088220C6064
          F80F65FD870930342FD80266026D5EF2FF1F8306C8C5602F30B3316ADED8F86A
          2990BD14DD564606466463C000E46C64354C2067210BFCFFFFFFC8DB376F500C
          FAF3E31B48E608D640C4880553B549B2FC0C5F7EBC7AAC6B20C428F45F82F9C9
          A5134F16FDFFFD6327360318676D3A3C879B8BD3E2DF7F06D11FBF7EF128880B
          31FCFCFDFB0DD0DD0F57AD5EB378517FE36C7C698321AE7DF1BDFF3840505655
          07BEB4A11D5D7B98E5FF7F064590A2151BB631ACDEB293E1EA4D487AB971782B
          C29938D20650C6061E0636E6C60C37EF3E60007A87E1C5EBB70CC402261863FD
          B6DD0CEACA0A0CA13EEE0C065AEA60B14F2C97CA4174798433839D020F832807
          6ADA403160EF91130C6A40037CDD1C195CEC2C2151F44D6C3388FEFCF91E43F7
          CA0C064E66CCB4C1C4C8C8701FC4F072B26388CA2A65D8B46B3F839BBD3558F2
          1FEB67751360FADF7A7EC15F37BD04AC69835940414B61CD81B3DF1E7FFCF55F
          405291898F97E7EFE3E7AFD9AEDE7DC470FBD6BB397BA7EC083CB4FE6C2227BB
          C0CB8F9FBF302CDCB8FBFDB3470F2F7C7EF77ACECFEF5FA601041800A8D14B27
          9403A03F0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_07'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000012C4944415478DA
          62644003C669336F02A960205E7B7656BA3A0301C082450CA4F928105B331001
          98B088AD05623F203E408C01D85C6007C4D7A134592E3804D5BC961803184F9E
          3CB911EA64385871EC094384950C31FA3731000DF80FE3C1D8C098F88FAE1259
          1D0C6CDCB8F13F23B2C4FBF7EF1904050519B2E65E6098966C80A21826870C1E
          3E7CC840910B56AD5AF51F1E0BEBD6ADFB2F2F2F0F97BC76ED1A5C8396961623
          36F12B57AE20A231282888F1D3A74F583521036471900BE0D178F7EEDDFF5356
          1F823B1F446FDA7BE2FF83070F509C0E72010CA3A403656565C6246F1348D440
          A3C8CFD9825141418111DD05308C9212416100A28119C8BF77C9EEFFD12EBA7E
          EF0A6CFF130A03ACB1800DE04B071B5FBC78E1F7F3E74F0652003B3B3B838484
          C426800003002355A0047442CEB10000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_08'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000A54944415478DA
          62FCFFFF3F0325800599533573C32120658B4DE1DF7F7F18FEFDFDCFF0E1E3C7
          C3B3AB53ECB01AF0EBE70FDB8C085FA0E2BF0C7FFEFE63F8FB174443D8BFA1EC
          290BD7DAE274C1AF9F3F190E5FBACBF0EBD76F86DFBFFF30FCFCFD0BC2FE0364
          03E95F7F7E33B0B1B0E0F6C2CF9F3FEFEFDCBD5B11AF9F59580E23F319910391
          919191A88043D683E282CA19EB890A44207734104703117720920300020C0074
          CFED1478F731A90000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_09'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000026A4944415478DA
          A4534B4F135114FE665A1E6D85B6D2960862343C62638CB23171A1892BE31F30
          0697C6C40D3121C6C41D18212C958D5413A236684412D32E941817049F0B076A
          8A047C50681928855286CE8CD3C770BD3313C6561263E2777332E7CC39E73B5F
          EE83C11FB83272FCC656C6D257EB568768F874F07CE415FE0266747C6AE2C8A1
          7DFEC307EA3D63B343084F3F44E7A97EF8EB4F22BEB6958FA532AC942B58771A
          E27C52ADDDEBBDF3E8E5875E543AB2B83AF02C4428DECD3F271787DBC9C8A701
          2D24A18F33845B5C270B12D965E3B33C69EBE80EE80A52996CC6EBDAE3BA307C
          14C9F92D34357B10ECE0F469627E1B32616161A85FA4A6E49116244C44BE2138
          FA025F1FF730CCD99B6D4475CB506805291832D51C812A138CDDE2C1E7AC9849
          6C60767119097E05F18538362405626A097323FD0C1BECE4369515A0F1601D1A
          5ADC70396BA0AC113CB9F605424ED5092BABAA60B3DB6173D4C0EEF6C05269A3
          DA593DC7AE0BA2F8A637013E2A405856909814301558D593EF17B3862442C0D0
          A5BB4668820D84DF4E6A8E46928C4AF8FCC068EE1A0C9945842939366D95C4BA
          0EB96850EE344F7033E634BD8932103322650AF4F32D1654AC48AAF9B3B5A519
          D5F6A8295BA72A1DCB50124BC56F8202CDCFC5E29064D9AC696B70D31D8FA1C9
          494B18AB2947FB14B31B1ACB3D93205F28C2E3AD87A7E48A3A7DFBB19ACE20F2
          9D47626919A9F534C49F39C8C2A6C8582BEE6F573B6F9B046A51C58F580C7289
          0263C3009F668D0ED85BBD38DDEEC7A5EB3DD3AFEFF67595ED419DCB8133278E
          8165CA1F8A4ACA639B65F763B246A6389CBBCC85F18FA8B3B1BE32958410FC0F
          7E0930005EC92419B60378BE0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_10'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000000BF4944415478DA
          62FCFFFF3F03450064000C838071DACC9B40AC03A2D1D502C536A2EB63C26266
          30101F85D2E89AFD900D01016C06AC052904E203E89AA15C5443B078411C88DF
          81BC81C50BFF89F1C22120B683BA84F44084DA74199B5A625D00023AC4C62213
          038580680340810BA45E51E20250E086A1273016120C8025306B9C0620271868
          88A79D9D953E1B2A0D4A58F1D0E855C7EA05A0627F20B509CADD84A419041C80
          78217A12C708039821501A3D895BA32730464AB333408001009C1D7050C1A8B0
          110000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_11'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000010A4944415478DA
          62FCFFFF3F03258085919191A12BD661233B3B87DFCF7B773695EDBBE30F92D0
          F44D0399FC0C88275EDF3CAB2B6F6BF04696BF0C7EC89AFF30336C62043126A6
          78FC7F2B65B6E9F7E52DDEACBA3E5B610A569F7F01D350CEEDBDD69A9F810DC5
          808F0CBF2006805CF0FECB276FC18F9FB6E272012E2F30C2187575751B9B9A9A
          FC7129C4E505166203EBD8E3030C402F30A07901C2003917E402A8B3498B0528
          FD0C1A60CFC8F5C24420EE84D2A47B01EA8D3272121223B10AB179E1FB9FAF9B
          583E65BB6FFCC7228C22C1F4E7ED26BEA93B51A2F4D2E3A30CCC68F681BCC0F2
          FCE2030621E63B2812EFFE62C6EE818C17FE0CB4008CC47A01673A20D60BB800
          408001008E92831C59F42B260000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_12'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002304944415478DA
          A4534B681351143D2FC924CDA789B1BA511782C1E2425070AF5908AE5C76A1E0
          B2B871E3A6B810046B10BA912CC406342A14348B8841A8A88B8A9F2AA6F5132A
          8DDF34BF76F2ED343371621AAFF36186A14D71E18537F3CECC3DE79D7BEF0C23
          2230C6608DB3894317D69AF6883FD8BBA5C0FB37463E3CC116C1360AA864C6FC
          914E9B1BB979E659225F15EA2B8234545B6B6BEF9BA23CCFECDCCCDDE9D92B70
          7A5B500592C9E443E54EAF7E3CA0D3538729918EAA90BEF202E524DAB466164B
          B4FFD4A54955D0A65EC2E1F049C5058BBEBE88E2F73A120BD7C5B96F656CF77B
          B1D305F81CBA3B51FE8D25BE89179F8B0AA251B384139787A9176C4316D7415D
          3DB9D721F4DA84E7133C520B0D2C2E9551282D239FCBA321C9102B4564135799
          E6E0DEB9B7909781DD7B87B02B14C4B6C020E42AE1F178099D1EE074B9E0F678
          E0F60EC213DC01BBD3AD1CAD51F5125AAD165E468AAC941120946514E605BC9F
          E4B58482A4BA551A0DBDD1A443333481743AAD0155642523E1D39D0A7B97CD63
          FACBAA4EB24C5915B24EDDB671AE1F6FF3E86633343EF51464CE9A297B03D166
          07D6A81D0B811B3EC8063C3ED3B666DC7A2C5344EC5C7F0133C7668BE5733FD1
          5C1574329952586F35D48C988A1D5652F5E8BEE3C67E4FE840F4C8A389D139E7
          79B1CC577D955A1DE2AF8ED270E90D7370B37F0602D77445FD4B24856CAC318B
          E098F13C168B515FAB8600FE11F178BC6F8E51424A4DE876BB7DC91CC7211008
          A4B6FC1BFF27FE0A3000D2602407342B9F600000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_13'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C0000012C4944415478DA
          62FCFFFF3F03232323033AA89CB1FE1090B265C002FEFEFBC3F0EFEF7F860F1F
          3F1E6661C0017EFDFC619B11E10B54FC97E1E7AFDF60B13F7FFF02F13F86DF60
          FA2FC394856B6DC106AC5BB76E2390F28369D6D2D26298BAE904C3E14B77197E
          01357FF9F60D622890FDFBCF1FB081BFFEFC666063616180B9C02F282808C51F
          69CD33EFEDDCBD5B91010F60616139CC000A03A00BFE3310003B76ECC0AA0667
          18A007E2C107DF4162FF691388D80DF8495220C2C16B7BE572DEBA891DF57BAF
          3100039181E440041A00677F5D34E53FA140640419B07EFDFAFFB0680419207A
          F02E23592911A8D995EC4044727A055981087236B26B7EFEFC799F9894083360
          D3FCF9F3FFFFFE0D71AA9C9C1C83AAAA2A22A080B9958787E7A99898980CD640
          A40400041800801007E70B356CB20000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_14'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001D84944415478DA
          62F8FFFF3F0321B06EED5A9D952B577E04B1172E5CF879F69CD93A3039469001
          8C8C8C38352F5CB0F0230F9FC055662616032626164E46C6FFDF5FBF7E799199
          89412B3E219E1F6CC0FAF5EB3702D5FA216BB4B3B36360626262B870E122C3F3
          E7EF18EEDEBDCBC0CACAC6202828C820212ECEF0E8F1EDA39F3E7DCA6081AAF7
          0B0A0AC270C6AA952B3F7370F233DFBE7D9B5345558D41804F70CADBF76FD2BE
          FFFCCEC6C6C665F4E7F7DBE34CF8FC1E161ECEFBFDFB4F4E6161510643035D06
          1F5FC7DC97AF5E2DE7E6E201BA8E8DB3A1B18117AF0120FFFFFDFBFBBBA0A000
          C3B97317185ADB26CC9110178BE5E0E06078F6ECC9B7F2B2F28F780DF8FDFBB7
          F5BB77AF2E30313132F0F0F032981A1B258B8B89335DBA7489E1D7CF2F17FEFC
          F96DCD82CF8094D4942B4057683F7D7AFF28D0C9D6FFFE31307CFCF8FEFBD7AF
          1FCEB0B2B118F6F6F55DC170C16B7BE57220FE0FC27F9E3DBE078AAA5FBF7E64
          006D64282DCD65FCF5FBCB5FA083B2BA3ABB78C11A40D1B86EDDBAFF48066065
          3F7BF60C6B8A636220126CD9B28581A001401B5DB178C9159FC14C68CEDD05C4
          1548F220F62E64AF6000F430C005E6CF9F8F550D2C1A37811400E31DAB665656
          56067E7EFE4DD8E41889C9CEF80040800100ACC5CC7E36FF64CD000000004945
          4E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_15'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001174944415478DA
          62FCFFFF3F032323230332A89CB1FE1090B265C002FEFEFBC3F0EFEF7F860F1F
          3F1E9E5D9D62C7824DD1AF9F3F6C33227C818AFF32FCF9FB8FE1EF5F100D61FF
          86B2A72C5C0BB60087013F190E5FBACBF0EBD76F86DFBFFF30FCFCFD0BC2FE03
          6403E95F7F7E33B0B140B46235E0E7CF9FF777EEDEADC88007B0B0B01C063340
          61402E00E9C5EA02620311C81DAC81C8FCE5C3A3DF8B7AE4821F1D03F373CC33
          7106222372427A6DAF2C09A46600F11610163D78F739D18188A439A32FB26725
          909E5519496420226B76D57F2FAAFA72ABEDEDB7EB185654DE273A10670035D6
          420D611062D367C8F3CD653870EE36D181E807A56B2F4C7A77C9AC99ED1E4929
          D1204FE83FB929919192A40C02000106008FB303723BE807E80000000049454E
          44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_16'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001464944415478DA
          6264C0022A67AC3F04A46CB1C9FDFDF787E1DFDFFF0C1F3E7E3C3CBB3AC58E05
          9BA25F3F7FD86644F80215FF65F8F3F71FC3DFBF201AC2FE0D654F59B8166C01
          0E037E321CBE7497E1D7AFDF0CBF7FFF61F8F9FB1784FD07C806D2BFFEFC6660
          6381686559B76EDD4620ED876CC0A5A79F1976EEDECD800FB0B0B01C06338006
          FC672011CC983103ACE7FFFFFFD8BD402810EFFCFECF90D2321BA46630042256
          677EFFFA081888722407E26B7B654920DE08C4A920362ECD13274EC40C44A886
          19409CD117D9B31248CFAA8CC41E888FFFA20522B26657FDF7A2CEA406E2375E
          8155931D4D979F7B79709D108338E981C8F5F983CDAD7717441DE582F6C9F1AA
          BD3CFBECA73A301005890944465020361D48612834EE5F4264427CFEFDFBF7D2
          8C8C0C4658206ECAD06AF3FBF0E1430C31BAD9D9D919C4C4C436C1F800010600
          46A81AE63715E4100000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_17'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002114944415478DA
          A4924B6813511486FF3B33191DC1D65925217651D082E2421B0A56893B1F0B15
          C9CA8DD4A25111DC49C55AE8A62E0CB50B5B418B05839BD4B7E94A021A8D5628
          821A022E4252688920B134D32433CEF33A33886899B18BFE70B9AF73FE73EEC7
          2594520CDE7DFE16400CAB94386E1FD9F7ED028B37B91CE2F13849A5520D4DD7
          7A1367134527863806976FA5E98593C7605A26544D7793032C415758C4C3741A
          E1C856341BB2ED454008556AB5EF5F58063BFB4EF7B5734EB0A6AAC817CAD0EC
          E4A62CBB06974EECC38A2421140EE35BB58672B98C408087288A422818DCBBB0
          587A3F32727D976BA0AAEAFCCB6CB6F3EFF62BD14E7CFD34870D1BDB502A95B0
          6D7B17B6B489134BCB3FCE29AAC2F3FCA66E435FFA00E709FFD3F4F40B3A3E3E
          4567665E8D3AFBE4E8EDFB994C964E4E3EA07F1878411CEE3F8AA78F1F81E504
          E8BA6517B2B0B0589DEAE888F48782216676F69DACC875E337839FB1D510398E
          C5A12387917B9D83691208C266F444BBCF38650B85829DD3FC6C18FA795F8873
          451EA70E46DD3B1B3D2A952A2C0B90A465A5D5AA7F0CF0DC9E9B6363455F888E
          F6EF8860776F0CAD1509F3950CBD3634C80C5C1968300417933792EE3F5813E2
          5A723BB87AE799E74F74645A062C93A22E49F97B4389039E065E100DD3B48705
          DD9D4D4CA49EC47C3BF082E8AC75C3700D354307CF71FE4FF083F84F20C7E5BD
          CEC97A213258A77E093000E72427B01E7BDCF70000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_18'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001914944415478DA
          62FCFFFF3F032580898142C0022218191971AB6064E4111315BDC806047FFFFE
          FDA59AD0A904147B75A82D561CE47AFC2E6064145056527A7AFAF46925A06699
          172F5F72FDFBF3EBC3CFCFEFC554E33A56C15D800B3033336F3F70E0009FACAC
          2CCCC05F2FDE7C2A6260F8BF8CE9CD6D76B018BE40D4D2D6FE2D2E2E0E52F05F
          5B5BFBBF87874709485C2330F7BA6A4CF35FB05E5C06B87907A69754D6FD6765
          65031B00B4BD0B595ED337ED0C482F2398400B4423332BF3F0AC8A1327DE3033
          085EDFB0E4F6AD1BF30E1F39721C68DB0F647520BD2861B06DBAF899E37BF80C
          FE3AA53183343BC9B2FC5FBDF976F3E1C3876F114C0720CD27F7F319585B7C65
          7EF29383C144F015C3C92D8B430E1D3C700B6F44819CB17D86C49923BB04746D
          DC3EB0317D63641097F66638B0E70043E19CBB8CF834C3D3C1C16DC25AE6AE72
          6CCCAC5A0CE2F25E0C270EEF675037FE7296E8A42C2CC1C3C1C1A9CDA0A61B0A
          D4BC8F415EE7EB59AFCC97264427E57FFFFEAFBA77F576F8BD2B3719E435BE13
          AD191E06039A1B01020C001A14ACD15503499E0000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_19'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001F64944415478DA
          62FCFFFF3F032580898142C0885F9691474C54F4221B10FCFDFBF7976A42A712
          50ECD5A1B658719034C8F54C78340B282B293D3D7DFAB41250B3CC8B972FB9FE
          FDF9F5E1E7E7F762AA711DAB60CA5870E9676666DE7EE0C0013E5959599881BF
          5EBCF95404B47719D39BDBEC04FDA6A5ADFD5B5C5C1C14C2FFB5B5B5FF7B7878
          9480C4350273AFABC634FF8579012B70F30E4C2FA9ACFBCFCACA063600687B17
          B2BCA66FDA1998011881686466651E9E5571E2C41B6606C1EB1B96DCBE7563DE
          E123478E0355FF40570B32001E06DBA68B9F39BE87CFE0AF531A3348B3932CCB
          FFD59B6F371F3E7CF816C17400D27C723F9F81B5C557E6273F39184C045F319C
          DCB238E4D0C103B708A60390E623BB04746DDC3EB0317D63641097F66638B0E7
          0043E19CBB8C843483BD70709BB09695A7341B33AB148398BC22C389FDFB1944
          8D15BF7C9E74F57FDBA2ED189AFEFEFBC3F0EFEF7F860F1F3F1E0672ED588425
          78383838B519D4B4D418762CEB6290D7F97E563768B5F18BCF7F18E2FD9D18FE
          FCFDC7004C4840FA2F98FD1BCA9EB270AD2D3821FDFBF77FD5BDABB7C3EF5DB9
          C920AFF1FDAC57E64B93DCCF9BFE1B1A1A32FCFAF59BE1F7EF3F0C3F7FFF82B0
          FF00D940FAD79FDF0C6C2C90F067A9987B3A024847203BF3E7CF9FF777EEDEAD
          88CFFF2C2C2C87C181486976060830005A09E73EB09F04920000000049454E44
          AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_20'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000002644944415478DA
          A4536D485351187EAEBB9BCB74E584AEB9722D4874D7C03028ABAD22581F4B52
          902614FDB22FD01FD18F8A3088C881FD2948B22CACA0FD084C28B21265B35916
          1A425F12B2ED87E9B00FDBDC74DDAFDDEEBDA66DB0F6C703E7BCE71CCEF39EE7
          3CE779095114B19846CA034110499BE75A3B5F4AC132BFDE5751AAC4424AAF74
          86E351DF7CD7DB76BECE4AA6CACA32BF2D276A2B21C40514EAB35077FC24789E
          878A2431BED28A278D07119A8E281764A44EC0C0FBDE07BD5685DDF603705E6C
          84B7F705863F7C468E1AB09DB905C71E2BFE9B806198C0F6120A3BED5570B5DF
          84D1B806C16010B3B3315C3D6AC3AF190E4BD522AF1C4E27A299A6398AA2E403
          224DD36220101893F78BAB1B46D61DBE24C85842195288189DFA6EC98C8EE3DA
          1527389EC3E3EE3E0CF87E4ABAF068A8D905C76967F4D5FDE69C9409EA9D6DA2
          695501FA2781A62A3326A66690939B0B517A312708E0A5BE7A5926D61A2822E9
          17BA6E5043033DBA325EA756C0172A693C1DF4499FAD063B3A2931E1C1B01C58
          89D1C4D8181E5C6E0099087EEBD6956DDD3CA372315A6CCCFD065DD6060C0E0F
          A73610497A150FC94F78D69A3FD4DFBD7CFD365B4893314B8032D8E1E9F1E0D4
          6D1F91CE85325661D0D79567DEB2D7A051A90BB0C268C21BB71BD9E515F8DA34
          24B63CF224816411E3828850382C339873625E7EB656BB844691B908CF5DCD20
          4A36A1E6483B26A76338B47F0784BFC2F1427C41C4EBF73A2C0BB5108F8B0FFD
          9F461DFE8F5F602C8EBD2BAD6E29EF7C3D0256128C937CCF70ECDC3C41440D49
          FE2BA6B377066BA5503B4FF3D88F0E7F381231A5ADC2441117D3FE0830009938
          16D7713105F20000000049454E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_21'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001584944415478DA
          62FCFFFF3F032580898142C00222DCD26A27DCFF2BEC0764F212AB5191F9EDD2
          5DB39A0B58A07C7325D677BCA4D8FCFF1F833984010C83FC49ABA7FFFBF7EFFF
          DFBF7F89C220B54159557B417A59E0263E9BC9C0F4620E98FDCFF01403D37933
          DCB64BA4A0860108FCE2776360615761F80F827FFF31308B27A1696304690522
          4686FF3C4640F66354039EB5CE6378DFD10A66AB7EFBC9C0279385D3057FFE82
          A27E23AA013CA1E10CCC5A5A408B18197EFCFAC370E7C5279C0688F072627A61
          FF6A6D867D1DBA6076F5EBCF0C468A22380DF8FDF73FA6014631FF19848CBE80
          D93F7EFF633877FF0D4E0384B1B94052E53F83AA360F987DF9D15B86B477A238
          0D8865AEC634E0F9BBAF0C9F7FFC864B787316E334408FD38EE110C341540364
          C4F8817E03660E4608BF94A107451392B71938991918A6201BF0F8D1C36B9EA9
          95334849CA9C2C8C8FC0A983D2EC0C10600016469D29E6C134A3000000004945
          4E44AE426082}
      end
      item
        ImageFormat = ifPNG
        ImageName = 'frntendtypesimages_22'
        ImgData = {
          89504E470D0A1A0A0000000D49484452000000100000001008060000001FF3FF
          610000000467414D410000AFC837058AE90000001974455874536F6674776172
          650041646F626520496D616765526561647971C9653C000001334944415478DA
          9452D16D83400C3D50164847482688F8E8029DA012152C005217202390052A7A
          0B2015A9EA3F374023156582640536C8F5B97D4827381A6AE961CE9C9FED8783
          244922A5D41BB051BFB6AFEBFAA0165A08C4408FA400BE014AF50F5B2171EF9C
          0D09D5EEF1F9EC74B53DBDBF5CBC04F248D354C6C888C19E802FA09B4BFE1901
          C90535904BB9F32DA26F6E6920550D858B9C6F0F83C7381628E608B490A09333
          BB90915E49D0B12B897B09026BED24886A31C7CA31BF7604BDC3B91F77E0B3A1
          7D83E435BCE0324EBE4560A87E46027D0CC302B0440B6C2623A0A224B7B291DC
          0B79EFAA8F4A73AC0335119DF295A77ACF0BC3466A74921FC3AAE5B9BBBF5E1B
          8AAF2604B82CCA6F3DC4EBB9DFB8D4FAD182296F077F98A1B819C4D38CC54A44
          5C8ACF2028014B9412FB166000145B96D0FAC176780000000049454E44AE4260
          82}
      end>
    Left = 240
    Top = 176
    Bitmap = {}
  end
  object AdvXMLMemoStyler1: TAdvXMLMemoStyler
    LineComment = '//'
    MultiCommentLeft = '<!--'
    MultiCommentRight = '-->'
    CommentStyle.TextColor = clNavy
    CommentStyle.BkColor = clWhite
    CommentStyle.Style = [fsItalic]
    NumberStyle.TextColor = clFuchsia
    NumberStyle.BkColor = clWhite
    NumberStyle.Style = [fsBold]
    AllStyles = <
      item
        KeyWords.Strings = (
          'val'
          'encoding'
          'xml version'
          ']]'
          '![CDATA['
          '![IGNORE['
          '![INCLUDE['
          '!ELEMENT'
          '!DOCTYPE'
          'PCDATA'
          'REQUIRED'
          'IMPLIED'
          'FIXED'
          'ENTITY'
          'NMTOKEN'
          'NMTOKENS'
          'NOTATION'
          'ID'
          'IDREF'
          'IDREFS'
          'xml:space'
          'xml:lang')
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stKeyword
        BracketStart = #0
        BracketEnd = #0
        Info = 'XML Default'
      end
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stBracket
        BracketStart = #39
        BracketEnd = #39
        Info = 'Single quote'
      end
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = [fsBold]
        BGColor = clWhite
        StyleType = stBracket
        BracketStart = '"'
        BracketEnd = '"'
        Info = 'Double Quote'
      end
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clTeal
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stSymbol
        BracketStart = #0
        BracketEnd = #0
        Symbols = ' =<?/>#'#13#10
        Info = 'XML delimiters'
      end>
    Description = 'XML Files (XML)'
    Filter = 'XML Files (*.xml)|*.xml'
    DefaultExtension = '.xml'
    StylerName = 'XML Files'
    Extensions = 'xml'
    Left = 80
    Top = 152
  end
  object AdvHTMLMemoStyler1: TAdvHTMLMemoStyler
    LineComment = '//'
    MultiCommentLeft = '<!--'
    MultiCommentRight = '-->'
    CommentStyle.TextColor = clSilver
    CommentStyle.BkColor = clWhite
    CommentStyle.Style = [fsItalic]
    NumberStyle.TextColor = clBlack
    NumberStyle.BkColor = clWhite
    NumberStyle.Style = []
    HighlightStyle.TextColor = clWhite
    HighlightStyle.BkColor = clRed
    HighlightStyle.Style = [fsBold]
    AllStyles = <
      item
        KeyWords.Strings = (
          '&lt'
          'ABREV'
          'ACRONYM'
          'ADDRESS'
          'APPLET'
          'AREA'
          'AU'
          'AUTHOR'
          'B'
          'BANNER'
          'BASE'
          'BASEFONT'
          'BGSOUND'
          'BIG'
          'BLINK'
          'BLOCQUOTE'
          'BODY'
          'BQ'
          'BR'
          'CAPTION'
          'CENTER'
          'CITE'
          'CODE'
          'COL'
          'COLGROUP'
          'COMMENT'
          'CREDIT'
          'DEL'
          'DFN'
          'DIR'
          'DIV'
          'DL'
          'DT'
          'DD'
          'EM'
          'EMBED'
          'FIG'
          'FN'
          'FONT'
          'FORM'
          'FRAME'
          'FRAMESET'
          'HEAD'
          'H1'
          'H2'
          'H3'
          'H4'
          'H5'
          'H6'
          'HR'
          'HTML'
          'I'
          'IFRAME'
          'IMG'
          'INPUT'
          'INS'
          'ISINDEX'
          'KBD'
          'LANG'
          'LH'
          'LI'
          'LINK'
          'LISTING'
          'MAP'
          'MARQUEE'
          'MATH'
          'MENU'
          'META'
          'MULTICOL'
          'NOBR'
          'NOFRAMES'
          'NOTE'
          'OL'
          'OVERLAY'
          'P'
          'PARAM'
          'PERSON'
          'PLAINTEXT'
          'PRE'
          'Q'
          'RANGE'
          'SAMP'
          'SCRIPT'
          'SELECT'
          'SMALL'
          'SPACER'
          'SPOT'
          'STRIKE'
          'STRONG'
          'SUB'
          'SUP'
          'TAB'
          'TBODY'
          'TEXTAREA'
          'TEXTFLOW'
          'TFOOT'
          'TH'
          'THEAD'
          'TITLE'
          'TT'
          'U'
          'UL'
          'VAR'
          'WBR'
          'XMP'
          'DOCTYPE'
          'PUBLIC')
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stKeyword
        BracketStart = #0
        BracketEnd = #0
        Info = 'HTML Standard Default'
      end
      item
        KeyWords.Strings = (
          'TABLE'
          'BORDER'
          'TD'
          'TR'
          'STYLE'
          'BORDERCOLOR'
          'WIDTH'
          'ID'
          'BORDERCOLORLIGHT'
          'BORDERCOLORDARK')
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clOlive
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stKeyword
        BracketStart = #0
        BracketEnd = #0
        Info = 'HTML Table Keywords'
      end
      item
        KeyWords.Strings = (
          'A'
          'HREF')
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stKeyword
        BracketStart = #0
        BracketEnd = #0
        Info = 'HTML Link Keywords'
      end
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBackground
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stBracket
        BracketStart = #39
        BracketEnd = #39
        Info = 'Simple Bracket'
      end
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stBracket
        BracketStart = '"'
        BracketEnd = '"'
        Info = 'Double Bracket'
      end
      item
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clTeal
        Font.Height = -11
        Font.Name = 'Courier New'
        Font.Style = []
        BGColor = clWhite
        StyleType = stSymbol
        BracketStart = #0
        BracketEnd = #0
        Symbols = ' ,;:.()[]=-*/^%<>#'#13#10
        Info = 'Symbols Delimiters'
      end>
    Description = 'Web pages'
    Filter = 'HTML Document (*.htm,*.html)|*.htm;*.html'
    DefaultExtension = '.html'
    StylerName = 'HTML document'
    Extensions = 'htm;html'
    Left = 80
    Top = 208
  end
end
