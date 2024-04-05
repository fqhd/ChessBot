import subprocess
import chess
import os
import random

names = os.listdir('versions')
a_path = f'versions/{names[0]}'
b_path = f'versions/{names[1]}'
print('A:', a_path)
print('B:', b_path)

def test_game(fen):
	board = chess.Board(fen)

	while True:
		process = subprocess.Popen([a_path, board.fen(), '4'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		output, _ = process.communicate()
		output = output.decode().strip()
		board.push_san(output)
		if board.is_checkmate():
			return 'a'
		if board.is_insufficient_material():
			return 'd'
		if board.is_repetition():
			return 'd'
		process = subprocess.Popen([b_path, board.fen(), '4'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		output, _ = process.communicate()
		output = output.decode().strip()
		board.push_san(output)
		if board.is_checkmate():
			return 'b'
		if board.is_insufficient_material():
			return 'd'
		if board.is_repetition():
			return 'd'
	
a_wins = 0
b_wins = 0
draws = 0

with open('fen_list.txt') as f:
	random_games = f.read().split('\n')
	random.shuffle(random_games)

for i, fen in enumerate(random_games):
	result = test_game(fen)
	if result == 'a':
		a_wins += 1
	elif result == 'b':
		b_wins += 1
	else:
		draws += 1
	print(f'Game {i}: {result}')

print('A Wins:', a_wins)
print('B Wins:', b_wins)
print('Draws:', draws)