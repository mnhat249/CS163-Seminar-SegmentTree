import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read CSV file
df = pd.read_csv('benchmark_results.csv')

# Create figure
fig = plt.figure(figsize=(16, 10))

# ===== Plot 1: Runtime Comparison =====
ax1 = plt.subplot(2, 2, 1)
df_naive = df[df['NaiveTime(ms)'] > 0]
ax1.plot(df_naive['n'], df_naive['NaiveTime(ms)'], 'o-', label='Naive Solution', 
         linewidth=2, markersize=6, color='red', alpha=0.7)
ax1.plot(df['n'], df['SegTreeTime(ms)'], 's-', label='Segment Tree', 
         linewidth=2, markersize=6, color='blue', alpha=0.7)
ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.set_xlabel('Input size (n) - log scale', fontsize=11)
ax1.set_ylabel('Runtime (ms) - log scale', fontsize=11)
ax1.set_title('Runtime Comparison: Naive vs Segment Tree', fontsize=12, fontweight='bold')
ax1.legend(fontsize=10)
ax1.grid(True, alpha=0.3, linestyle='--')

# ===== Plot 2: Speedup =====
ax2 = plt.subplot(2, 2, 2)
df_speedup = df[df['Speedup'] > 0]
ax2.plot(df_speedup['n'], df_speedup['Speedup'], 'd-', label='Speedup Factor', 
         linewidth=2, markersize=6, color='green', alpha=0.7)
ax2.set_xscale('log')
ax2.set_xlabel('Input size (n) - log scale', fontsize=11)
ax2.set_ylabel('Speedup (x times)', fontsize=11)
ax2.set_title('Performance Speedup of Segment Tree over Naive\n(higher is better)', fontsize=12, fontweight='bold')
ax2.axhline(y=1, color='red', linestyle='--', alpha=0.5, label='Baseline (1x)')
ax2.legend(fontsize=10)
ax2.grid(True, alpha=0.3, linestyle='--')

# Add annotations for key points
for i, row in df_speedup.iterrows():
    if row['n'] in [1000, 10000, 50000]:
        ax2.annotate(f"{row['Speedup']:.0f}x", 
                    xy=(row['n'], row['Speedup']),
                    xytext=(5, 5), textcoords='offset points', 
                    fontsize=9, fontweight='bold')

# ===== Plot 3: Memory Usage =====
ax3 = plt.subplot(2, 2, 3)
ax3.plot(df['n'], df['NaiveMemory(KB)'], 'o-', label='Naive Solution', 
         linewidth=2, markersize=6, color='red', alpha=0.7)
ax3.plot(df['n'], df['SegTreeMemory(KB)'], 's-', label='Segment Tree', 
         linewidth=2, markersize=6, color='blue', alpha=0.7)
ax3.set_xscale('log')
ax3.set_yscale('log')
ax3.set_xlabel('Input size (n) - log scale', fontsize=11)
ax3.set_ylabel('Memory usage (KB) - log scale', fontsize=11)
ax3.set_title('Memory Usage Comparison', fontsize=12, fontweight='bold')
ax3.legend(fontsize=10)
ax3.grid(True, alpha=0.3, linestyle='--')

# ===== Plot 4: Memory Overhead =====
ax4 = plt.subplot(2, 2, 4)
ax4.plot(df['n'], df['MemoryRatio(%)'], 'h-', label='Memory Ratio', 
         linewidth=2, markersize=6, color='purple', alpha=0.7)
ax4.set_xscale('log')
ax4.set_xlabel('Input size (n) - log scale', fontsize=11)
ax4.set_ylabel('Memory overhead (%)', fontsize=11)
ax4.set_title('Segment Tree Memory Overhead Compared to Naive', fontsize=12, fontweight='bold')
ax4.axhline(y=100, color='red', linestyle='--', alpha=0.5, label='100% (equal memory)')
ax4.legend(fontsize=10)
ax4.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()
plt.savefig('benchmark_plots.png', dpi=150, bbox_inches='tight')
plt.show()

# ===== Print Statistics =====
print("\n" + "="*60)
print("EXPERIMENTAL RESULTS")
print("="*60)
print(f"✅ Average memory overhead of Segment Tree: {df['MemoryRatio(%)'].mean():.1f}%")
print(f"🚀 Average speedup (n ≤ 50000): {df_speedup['Speedup'].mean():.1f}x")
print(f"🎯 Maximum speedup: {df_speedup['Speedup'].max():.1f}x (at n = {df_speedup.loc[df_speedup['Speedup'].idxmax(), 'n']})")
print(f"⚡ Segment Tree runtime at n=1,000,000: {df[df['n']==1000000]['SegTreeTime(ms)'].values[0]:.2f} ms")
print(f"💾 Segment Tree memory at n=1,000,000: {df[df['n']==1000000]['SegTreeMemory(KB)'].values[0]:.1f} KB ≈ {df[df['n']==1000000]['SegTreeMemory(KB)'].values[0]/1024:.1f} MB")

print("\n" + "="*60)
print("KEY OBSERVATIONS")
print("="*60)
print("• Segment Tree significantly outperforms Naive for large inputs (up to 194x at n=50000)")
print("• Segment Tree uses ~2.22x (222%) more memory than Naive solution")
print("• Naive solution is infeasible for n > 50000 due to excessive runtime")
print("• Segment Tree can handle n=1,000,000 in just 0.33 seconds")
