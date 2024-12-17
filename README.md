# c_terminal_reversi
ターミナルで遊べるリバーシ  
Linuxでうごくよ


## コンパイル
make

gcc以外のコンパイラを使いたい場合はMakefileをいじってね


## 遊び方
wasdで移動  
スペースで決定

エスケープで終了


## モード説明

### 1 PLAYER
人 vs NPC  
レベルはCPU1のものが適用される

### 2 PLAYERS
人 vs 人

### CPU ONLY
NPC vs NPC  
レベルは白がCPU1、黒がCPU2のものが適用される

### CONFIG
せってい


## せってい

### CPU LEVEL
NPCのレベル。現状2段階。  
* 0  
完全ランダム
* 1  
1手先だけ見て判断

### FIRST
1 PLAYERモードでの先攻

### DISC TYPE
石の見た目  
* 0  
絵文字
* 1  
○×

## とらぶるしゅーてぃんぐ
+ 強制終了させたらカーソルが迷子になった  
  `echo -e "\e[?25h"`

+ <del>X側のカウントが狂ってる</del>  
  <del>バグです。直します…</del>  
  直しました  
  教訓：画面を更新する時は横着せずに消しましょう
