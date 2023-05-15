from enum import Enum, auto

# マスの状態
class BoardState(Enum):
    BLANK = auto()
    PLAYER = auto()
    AI = auto()
    
# ゲームの状態
class GameState(Enum):
    GAME = auto()
    PLAYER_WIN = 'PLAYER_WIN'
    AI_WIN = 'AI_WIN'
    DRAW = 'DRAW'

class Board():
    
    def __init__(self):
        # ボードの初期化
        self.board = [BoardState.BLANK for _ in range(9)]
        # 先攻後攻
        self.my_turn = True
        
        print('0|1|2\n-----\n3|4|5\n-----\n6|7|8\n')
                
        # ゲーム開始
        self.state = GameState.GAME
        while self.state == GameState.GAME:
            if self.my_turn:
                # 先攻
                self.player_input()
            else:
                # 後攻
                self.ai_think()
            self.display_board()
        print(self.state)
        
    # ボードを表示する
    def display_board(self):
        tmp = []
        for i in range(9):
            if self.board[i] == BoardState.BLANK:
                tmp.append('  ')
            elif self.board[i] == BoardState.PLAYER:
                tmp.append('o')
            elif self.board[i] == BoardState.AI:
                tmp.append('x')
                
        print('{0[0]}|{0[1]}|{0[2]}\n'\
        '-----\n{0[3]}|{0[4]}|{0[5]}\n'\
        '-----\n{0[6]}|{0[7]}|{0[8]}\n'.format(tmp))
        
    # プレイヤーの入力
    def player_input(self):
        while True:
            try:
                value = int(input('please input value'))
                if self.can_put_value(value):
                    self.put_value(value)
                    break
                else:
                    print('do not put')
            except:
                print('attribute error')
    
    # ターンを交代する
    def change_turn(self):
        self.my_turn = not(self.my_turn)
        
    # valueがBLANKならTrue
    def can_put_value(self, value):
        return True if self.board[value] == BoardState.BLANK else False
    
    # valueに置く
    def put_value(self, value):
        self.board[value] = BoardState.PLAYER if self.my_turn else BoardState.AI
        self.check_state()
        self.change_turn()

    # 勝敗がついているならTrue
    def judge(self, a, b, c):
          return True if a == b == c and a != BoardState.BLANK else False
    
    # ゲームの状態を更新
    def check_state(self):
        for i in range(3):
            if self.judge(*self.board[i:9:3]) or self.judge(*self.board[3*i:3*i+3]) or self.judge(*self.board[0:9:4]) or self.judge(*self.board[2:7:2]):
                if self.my_turn:
                    self.state = GameState.PLAYER_WIN
                    return
                else:
                    self.state = GameState.AI_WIN
                    return
                
        if all(BoardState.BLANK != state for state in self.board):
            self.state = GameState.DRAW
            return
        
        self.state = GameState.GAME
        
        
     
    # ----- ここからAI ----- #
    
    # valueを空にする
    def undo_value(self, value):
        self.board[value] = BoardState.BLANK
        self.change_turn()    
    
    # 次の手を考える
    def ai_think(self):
        print("AI")
        self.put_value(self.minimax(0))

    # ミニマックス法で探索
    def minimax(self, depth):
        if self.state != GameState.GAME:
            return self.evaluate(depth)
        
        best_value = 0
        value = 10000 if self.my_turn else -10000
        
        for i in range(9):
            if self.can_put_value(i):
                self.put_value(i)
                child_value = self.minimax(depth+1)
                if self.my_turn:
                    if child_value > value:
                        value = child_value
                        best_value = i
                else:
                    if child_value < value:
                        value = child_value
                        best_value = i
                self.undo_value(i)
        
        if depth == 0:
            return best_value
        else:
            return value
        
    # 評価関数
    def evaluate(self, depth):
        if self.state == GameState.AI_WIN:
            self.state = GameState.GAME
            return 10 - depth
        elif self.state == GameState.PLAYER_WIN:
            self.state = GameState.GAME
            return depth -10
        else:
            self.state = GameState.GAME
            return 0
        
Board()