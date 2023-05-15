from enum import Enum, auto

class BoardState(Enum):
    BLANK = auto()
    PLAYER = auto()
    CPU = auto()
    
class GameState(Enum):
    GAME = auto()
    PLAYER_WIN = 'PLAYER_WIN'
    CPU_WIN = 'CPU_WIN'
    DRAW = 'DRAW'

class Board():   
    def __init__(self):
        self.board = [BoardState.BLANK for _ in range(9)]
        self.turn = True      
        print('0|1|2')
        print('3|4|5')
        print('6|7|8')               
        self.state = GameState.GAME
        while self.state == GameState.GAME:
            if self.turn:
                self.player()
            else:
                self.cpu()
            self.display()
        if self.state == GameState.PLAYER_WIN:
            print('Player Win!')
        elif self.state == GameState.CPU_WIN:
            print('CPU Win')
        else:
            print('Draw')
        
    def display(self):
        tmp = []
        for i in range(9):
            if self.board[i] == BoardState.BLANK:
                tmp.append(' ')
            elif self.board[i] == BoardState.PLAYER:
                tmp.append('o')
            elif self.board[i] == BoardState.CPU:
                tmp.append('x')             
        print(tmp[0] + '|' + tmp[1] + '|' + tmp[2])
        print(tmp[3] + '|' + tmp[4] + '|' + tmp[5])
        print(tmp[6] + '|' + tmp[7] + '|' + tmp[8])
        print("")
    
    def player(self):
        while True:
            try:
                value = int(input('input:'))
                if self.board[value] == BoardState.BLANK:
                    self.put(value)
                    break
                else:
                    print('do not put')
            except:
                print('error')
 
    def judge(self, a, b, c):
          if a == b == c and a != BoardState.BLANK:
              return True
          else:
              False    
 
    def put(self, value):
        if self.turn:
            self.board[value] = BoardState.PLAYER
        else:
            self.board[value] = BoardState.CPU
        for i in range(3):
            if self.judge(*self.board[i:9:3]) or self.judge(*self.board[3*i:3*i+3]) or self.judge(*self.board[0:9:4]) or self.judge(*self.board[2:7:2]):
                if self.turn:
                    self.state = GameState.PLAYER_WIN
                else:
                    self.state = GameState.CPU_WIN
                self.turn = not(self.turn)
                return 
        if all(BoardState.BLANK != state for state in self.board):
            self.state = GameState.DRAW
            self.turn = not(self.turn)
            return 
        self.state = GameState.GAME
        self.turn = not(self.turn)

    def cpu(self):
        print("CPU")
        self.put(self.minimax(0))

    def minimax(self, depth):
        if self.state != GameState.GAME:
            return self.evaluate(depth)      
        best_value = 0
        if self.turn:
            value = 10000
        else:
            value = -10000
        for i in range(9):
            if self.board[i] == BoardState.BLANK:
                self.put(i)
                child_value = self.minimax(depth + 1)
                if self.turn:
                    if child_value > value:
                        value = child_value
                        best_value = i
                elif child_value < value:
                    value = child_value
                    best_value = i
                self.board[i] = BoardState.BLANK
                self.turn = not(self.turn)
        if depth == 0:
            return best_value
        else:
            return value
        
    def evaluate(self, depth):
        if self.state == GameState.CPU_WIN:
            self.state = GameState.GAME
            return 10 - depth
        elif self.state == GameState.PLAYER_WIN:
            self.state = GameState.GAME
            return depth - 10
        else:
            self.state = GameState.GAME
            return 0
        
Board()