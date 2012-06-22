ファイルの説明

・AutoBinフォルダ…パソコン側で実行したexeファイルがあります。
   > SerialControl.exe は標準入力からコマンドを受け取ってシリアル通信を用いて
		Arduinoに送るプログラム
   >VoiceReconginition.exeはJuliusをモジュールモードで起動(http://julius.sourceforge.jp/juliusbook/ja/desc_module.html)したものと通信して、認識結果をもとにArduinoに命令を渡すプログラムです。
  >FaceRecongnition.exe顔認識プログラムです。learn_vv6_0.5.txtは超平面のデータです。


・Arduinoフォルダ…Arduinoのマイコンプログラムのコードが入っています。

・Juliusフォルダ…Juliusの設定ファイルです。

・srcフォルダ…ソースコードが入っています。
  >FaceRecognMain.cppがFaceRecongnition.exeのmain関数があるもので、
  >VoiceRecognMain.cppがVoiceReconginition.exeのmain関数があるもので、
  >SerialControlMain.cppがSerialControl.exeのmain関数が書かれているファイルです。
　 >tinyxml.h、tinyxml.cppはhttp://sourceforge.net/projects/tinyxml/から手に入るフリーのxmlパーサーです。



・RecogSlide.pptxは発表の時使用したスライドです。



