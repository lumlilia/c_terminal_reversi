# c_terminal_reversi
ターミナルで遊べるリバーシ(まだ未完成)  
Linuxでうごくよ

## 遊び方
wasdで移動  
スペースで石を置く

エスケープで終了

## とらぶるしゅーてぃんぐ
+ 強制終了させたらカーソルが迷子になった  
  `echo -e "\e[?25h"`

+ <del>X側のカウントが狂ってる</del>  
  <del>バグです。直します…</del>  
  直しました  
  教訓：画面を更新する時は横着せずに消しましょう
