import pandas as pd
import matplotlib.pyplot as plt

# 读取数据
# 注意：请确保blocksize_results.csv在同一目录下

df = pd.read_csv('blocksize_results.csv')

# 转换为数值型
df['block_size'] = df['block_size'].astype(int)
df['time_ms'] = pd.to_numeric(df['time_ms'], errors='coerce')
df['cycles'] = pd.to_numeric(df['cycles'], errors='coerce')
df['instructions'] = pd.to_numeric(df['instructions'], errors='coerce')
df['cache_misses'] = pd.to_numeric(df['cache_misses'], errors='coerce')

# 绘制执行时间
plt.figure(figsize=(10,6))
plt.plot(df['block_size'], df['time_ms'], marker='o', label='Vector-at-a-time (ms)')
plt.xlabel('BLOCK_SIZE')
plt.ylabel('Time (ms)')
plt.title('Time vs BLOCK_SIZE')
plt.xscale('log')
plt.xticks(df['block_size'], df['block_size'])
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig('../plots/blocksize_plot_time.png')

# 绘制IPC
plt.figure(figsize=(10,6))
ipc = df['instructions'] / df['cycles']
plt.plot(df['block_size'], ipc, marker='o', color='orange', label='IPC')
plt.xlabel('BLOCK_SIZE')
plt.ylabel('IPC')
plt.title('IPC vs BLOCK_SIZE')
plt.xscale('log')
plt.xticks(df['block_size'], df['block_size'])
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig('../plots/blocksize_plot_ipc.png')

# 绘制cache miss
plt.figure(figsize=(10,6))
plt.plot(df['block_size'], df['cache_misses'], marker='o', color='red', label='Cache Misses')
plt.xlabel('BLOCK_SIZE')
plt.ylabel('Cache Misses')
plt.title('Cache Misses vs BLOCK_SIZE')
plt.xscale('log')
plt.xticks(df['block_size'], df['block_size'])
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig('../plots/blocksize_plot_cache_misses.png')