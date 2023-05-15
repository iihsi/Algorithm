import numpy as np
from matplotlib import pyplot as plt

a = 1
b = 1
c = 1
x = np.arange(-5, 5, 0.2)
noise = np.random.normal(loc=0, scale=2, size=len(x)) 
y = a * x**2 + b * x + c + noise
 
coe = np.polyfit(x, y, 2)
print(coe)
 
y_fit = coe[0] * x ** 2 + coe[1] * x + coe[2]

# 相関係数と決定係数を計算する
r = np.corrcoef(y, y_fit)[0,1] # 相関係数R
r2 = r ** 2 # 決定係数R^2
print(r, r2)

plt.rcParams['font.size'] = 14
plt.rcParams['font.family'] = 'Times New Roman'

# 目盛を内側にする。
plt.rcParams['xtick.direction'] = 'in'
plt.rcParams['ytick.direction'] = 'in'

# グラフの上下左右に目盛線を付ける。
fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.yaxis.set_ticks_position('both')
ax1.xaxis.set_ticks_position('both')

# 軸のラベルを設定する。
ax1.set_xlabel('x')
ax1.set_ylabel('y')

# データプロットの準備。
ax1.scatter(x, y, label='sample', lw=1, marker="o")
ax1.plot(x, y_fit, label='fitted curve', lw=1)

# グラフを表示する。
fig.tight_layout()
plt.show()
plt.close()