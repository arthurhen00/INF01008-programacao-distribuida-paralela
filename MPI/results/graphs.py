import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import warnings
import os
warnings.filterwarnings('ignore')

csv = f"./merged_results.csv"
df = pd.read_csv(csv)

os.makedirs('./images', exist_ok=True)

sns.set_style("whitegrid")

def calculate_speedup(df_subset, algo):
    """Calculate speedup relative to single node execution"""
    algo_data = df_subset[df_subset['algorithm'] == algo].groupby('nodes')['total_time'].mean()
    baseline = algo_data[1]  # single node time
    speedup = baseline / algo_data
    return speedup

sizes = sorted(df['n'].unique())
colors = {'mpi_coletiva': '#1f77b4', 'mpi_p2p_bloq': '#ff7f0e', 'mpi_p2p_nb': '#2ca02c'}

# SPEEDUP GRAPHS (with ideal speedup reference)
fig1, axes = plt.subplots(2, 2, figsize=(15, 12))
fig1.suptitle('Speedup Analysis vs Node Count (with Ideal Linear Speedup Reference)', 
              fontsize=16, fontweight='bold')

for idx, n in enumerate(sizes):
    ax = axes[idx // 2, idx % 2]
    df_size = df[df['n'] == n]
    
    nodes_vals = sorted(df_size['nodes'].unique())
    ideal_speedup = nodes_vals
    ax.plot(nodes_vals, ideal_speedup, 'k--', linewidth=2.5, label='Ideal (Linear)', marker='^')
    
    for algo in sorted(df_size['algorithm'].unique()):
        speedup = calculate_speedup(df_size, algo)
        ax.plot(speedup.index, speedup.values, marker='o', label=algo, 
               linewidth=2.5, color=colors[algo], markersize=8)
    
    ax.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax.set_ylabel('Speedup', fontsize=11, fontweight='bold')
    ax.set_title(f'Data Size n={n}', fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=9)
    ax.set_xticks(nodes_vals)
    ax.set_ylim([0.9, max(ideal_speedup) + 0.5])
    
    # Save individual subplot
    fig_single = plt.figure(figsize=(8, 6))
    ax_single = fig_single.add_subplot(111)
    ax_single.plot(nodes_vals, ideal_speedup, 'k--', linewidth=2.5, label='Ideal (Linear)', marker='^')
    for algo in sorted(df_size['algorithm'].unique()):
        speedup = calculate_speedup(df_size, algo)
        ax_single.plot(speedup.index, speedup.values, marker='o', label=algo, 
                      linewidth=2.5, color=colors[algo], markersize=8)
    ax_single.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax_single.set_ylabel('Speedup', fontsize=11, fontweight='bold')
    ax_single.set_title(f'Speedup Analysis - Data Size n={n}', fontsize=12, fontweight='bold')
    ax_single.grid(True, alpha=0.3)
    ax_single.legend(fontsize=10)
    ax_single.set_xticks(nodes_vals)
    ax_single.set_ylim([0.9, max(ideal_speedup) + 0.5])
    fig_single.tight_layout()
    fig_single.savefig(f'./images/01_speedup_n{n}.png', dpi=300, bbox_inches='tight')
    plt.close(fig_single)

plt.tight_layout()
plt.savefig('./images/01_speedup_analysis.png', dpi=300, bbox_inches='tight')
plt.show()


# EFFICIENCY GRAPH

fig2, axes = plt.subplots(2, 2, figsize=(15, 12))
fig2.suptitle('Efficiency Analysis: Speedup / Number of Nodes', 
              fontsize=16, fontweight='bold')

for idx, n in enumerate(sizes):
    ax = axes[idx // 2, idx % 2]
    df_size = df[df['n'] == n]
    
    # Ideal efficiency (100%)
    nodes_vals = sorted(df_size['nodes'].unique())
    ax.axhline(y=1.0, color='k', linestyle='--', linewidth=2.5, label='Ideal (100%)')
    
    
    for algo in sorted(df_size['algorithm'].unique()):
        speedup = calculate_speedup(df_size, algo)
        efficiency = speedup / speedup.index  # speedup / num_nodes
        ax.plot(efficiency.index, efficiency.values, marker='o', label=algo, 
               linewidth=2.5, color=colors[algo], markersize=8)
    
    ax.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax.set_ylabel('Efficiency (Speedup / Nodes)', fontsize=11, fontweight='bold')
    ax.set_title(f'Data Size n={n}', fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=9)
    ax.set_xticks(nodes_vals)
    ax.set_ylim([0, 1.2])
    
    # Save individual subplot
    fig_single = plt.figure(figsize=(8, 6))
    ax_single = fig_single.add_subplot(111)
    ax_single.axhline(y=1.0, color='k', linestyle='--', linewidth=2.5, label='Ideal (100%)')
    for algo in sorted(df_size['algorithm'].unique()):
        speedup = calculate_speedup(df_size, algo)
        efficiency = speedup / speedup.index
        ax_single.plot(efficiency.index, efficiency.values, marker='o', label=algo, 
                      linewidth=2.5, color=colors[algo], markersize=8)
    ax_single.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax_single.set_ylabel('Efficiency (Speedup / Nodes)', fontsize=11, fontweight='bold')
    ax_single.set_title(f'Efficiency Analysis - Data Size n={n}', fontsize=12, fontweight='bold')
    ax_single.grid(True, alpha=0.3)
    ax_single.legend(fontsize=10)
    ax_single.set_xticks(nodes_vals)
    ax_single.set_ylim([0, 1.2])
    fig_single.tight_layout()
    fig_single.savefig(f'./images/02_efficiency_n{n}.png', dpi=300, bbox_inches='tight')
    plt.close(fig_single)

plt.tight_layout()
plt.savefig('./images/02_efficiency_analysis.png', dpi=300, bbox_inches='tight')
plt.show()

# BOXPLOT - TOTAL TIME COMPARISON

fig3, axes = plt.subplots(2, 2, figsize=(16, 12))
fig3.suptitle('Total Time Distribution by Algorithm and Node Count', 
              fontsize=16, fontweight='bold')

for idx, n in enumerate(sizes):
    ax = axes[idx // 2, idx % 2]
    df_size = df[df['n'] == n]
    
    sns.boxplot(data=df_size, x='nodes', y='total_time', hue='algorithm', ax=ax, palette='Set2')
    ax.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax.set_ylabel('Total Time (seconds)', fontsize=11, fontweight='bold')
    ax.set_title(f'Data Size n={n}', fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3, axis='y')
    ax.legend(title='Algorithm', fontsize=9, title_fontsize=10)
    
    # Save individual subplot
    fig_single = plt.figure(figsize=(10, 6))
    ax_single = fig_single.add_subplot(111)
    sns.boxplot(data=df_size, x='nodes', y='total_time', hue='algorithm', ax=ax_single, palette='Set2')
    ax_single.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax_single.set_ylabel('Total Time (seconds)', fontsize=11, fontweight='bold')
    ax_single.set_title(f'Total Time Distribution - Data Size n={n}', fontsize=12, fontweight='bold')
    ax_single.grid(True, alpha=0.3, axis='y')
    ax_single.legend(title='Algorithm', fontsize=10, title_fontsize=11)
    fig_single.tight_layout()
    fig_single.savefig(f'./images/03_total_time_n{n}.png', dpi=300, bbox_inches='tight')
    plt.close(fig_single)

plt.tight_layout()
plt.savefig('./images/03_total_time_boxplot.png', dpi=300, bbox_inches='tight')
plt.show()


# BOXPLOT - COMMUNICATION TIME COMPARISON
fig4, axes = plt.subplots(2, 2, figsize=(16, 12))
fig4.suptitle('Communication Time Distribution by Algorithm and Node Count', 
              fontsize=16, fontweight='bold')

for idx, n in enumerate(sizes):
    ax = axes[idx // 2, idx % 2]
    df_size = df[df['n'] == n]
    
    sns.boxplot(data=df_size, x='nodes', y='comm_time', hue='algorithm', ax=ax, palette='Set2')
    ax.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax.set_ylabel('Communication Time (seconds)', fontsize=11, fontweight='bold')
    ax.set_title(f'Data Size n={n}', fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3, axis='y')
    ax.legend(title='Algorithm', fontsize=9, title_fontsize=10)
    
    # Save individual subplot
    fig_single = plt.figure(figsize=(10, 6))
    ax_single = fig_single.add_subplot(111)
    sns.boxplot(data=df_size, x='nodes', y='comm_time', hue='algorithm', ax=ax_single, palette='Set2')
    ax_single.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax_single.set_ylabel('Communication Time (seconds)', fontsize=11, fontweight='bold')
    ax_single.set_title(f'Communication Time Distribution - Data Size n={n}', fontsize=12, fontweight='bold')
    ax_single.grid(True, alpha=0.3, axis='y')
    ax_single.legend(title='Algorithm', fontsize=10, title_fontsize=11)
    fig_single.tight_layout()
    fig_single.savefig(f'./images/04_comm_time_n{n}.png', dpi=300, bbox_inches='tight')
    plt.close(fig_single)

plt.tight_layout()
plt.savefig('./images/04_comm_time_boxplot.png', dpi=300, bbox_inches='tight')

plt.show()


# BOXPLOT - COMPUTATION TIME COMPARISON

fig5, axes = plt.subplots(2, 2, figsize=(16, 12))
fig5.suptitle('Computation Time Distribution by Algorithm and Node Count', 
              fontsize=16, fontweight='bold')

for idx, n in enumerate(sizes):
    ax = axes[idx // 2, idx % 2]
    df_size = df[df['n'] == n]
    
    sns.boxplot(data=df_size, x='nodes', y='comp_time', hue='algorithm', ax=ax, palette='Set2')
    ax.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax.set_ylabel('Computation Time (seconds)', fontsize=11, fontweight='bold')
    ax.set_title(f'Data Size n={n}', fontsize=12, fontweight='bold')
    ax.grid(True, alpha=0.3, axis='y')
    ax.legend(title='Algorithm', fontsize=9, title_fontsize=10)
    
    # Save individual subplot
    fig_single = plt.figure(figsize=(10, 6))
    ax_single = fig_single.add_subplot(111)
    sns.boxplot(data=df_size, x='nodes', y='comp_time', hue='algorithm', ax=ax_single, palette='Set2')
    ax_single.set_xlabel('Number of Nodes', fontsize=11, fontweight='bold')
    ax_single.set_ylabel('Computation Time (seconds)', fontsize=11, fontweight='bold')
    ax_single.set_title(f'Computation Time Distribution - Data Size n={n}', fontsize=12, fontweight='bold')
    ax_single.grid(True, alpha=0.3, axis='y')
    ax_single.legend(title='Algorithm', fontsize=10, title_fontsize=11)
    fig_single.tight_layout()
    fig_single.savefig(f'./images/05_comp_time_n{n}.png', dpi=300, bbox_inches='tight')
    plt.close(fig_single)

plt.tight_layout()
plt.savefig('./images/05_comp_time_boxplot.png', dpi=300, bbox_inches='tight')
plt.show()