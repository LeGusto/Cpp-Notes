#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

// Iterative DP solution for coin change problem
// Problem: Given coins of different denominations and a total amount,
// find the minimum number of coins needed to make that amount

class CoinChange {
private:
    std::vector<int> coins;

public:
    CoinChange(std::vector<int>& coins) : coins(coins) {}
    
    // Bottom-up DP: dp[i] = minimum coins needed to make amount i
    int coinChange(int amount) {
        // dp[i] represents minimum coins needed to make amount i
        std::vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0;  // Base case: 0 coins needed for amount 0
        
        // Fill dp array from 1 to amount
        for (int i = 1; i <= amount; i++) {
            // Try each coin
            for (int coin : coins) {
                if (coin <= i && dp[i - coin] != INT_MAX) {
                    dp[i] = std::min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        
        return (dp[amount] == INT_MAX) ? -1 : dp[amount];
    }
    
    // Count number of ways to make amount (order matters)
    // This counts all combinations, including different orders
    long long countWaysOrdered(int amount) {
        std::vector<long long> dp(amount + 1, 0);
        dp[0] = 1;  // One way to make 0: use no coins
        
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (coin <= i) {
                    dp[i] += dp[i - coin];
                }
            }
        }
        
        return dp[amount];
    }
    
    // Count number of distinct combinations (order doesn't matter)
    // This counts only unique combinations, not permutations
    long long countWaysUnordered(int amount) {
        std::vector<long long> dp(amount + 1, 0);
        dp[0] = 1;
        
        // Process coins one at a time to avoid counting permutations
        for (int coin : coins) {
            for (int i = coin; i <= amount; i++) {
                dp[i] += dp[i - coin];
            }
        }
        
        return dp[amount];
    }
    
    // Optional: reconstruct the solution (which coins were used)
    std::vector<int> reconstructSolution(int amount) {
        std::vector<int> dp(amount + 1, INT_MAX);
        std::vector<int> parent(amount + 1, -1);  // Track which coin was used
        
        dp[0] = 0;
        
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (coin <= i && dp[i - coin] != INT_MAX) {
                    if (dp[i - coin] + 1 < dp[i]) {
                        dp[i] = dp[i - coin] + 1;
                        parent[i] = coin;
                    }
                }
            }
        }
        
        // Reconstruct path
        std::vector<int> solution;
        if (dp[amount] == INT_MAX) {
            return solution;  // No solution
        }
        
        int current = amount;
        while (current > 0) {
            int coin = parent[current];
            solution.push_back(coin);
            current -= coin;
        }
        
        return solution;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    int n, amount;
    std::cin >> n >> amount;
    
    std::vector<int> coins(n);
    for (int i = 0; i < n; i++) {
        std::cin >> coins[i];
    }
    
    CoinChange cc(coins);
    
    // Find minimum coins needed
    int result = cc.coinChange(amount);
    
    if (result == -1) {
        std::cout << "Cannot make amount with given coins\n";
    } else {
        std::cout << "Minimum coins needed: " << result << "\n";
        
        // Optional: show which coins were used
        std::vector<int> solution = cc.reconstructSolution(amount);
        if (!solution.empty()) {
            std::cout << "Coins used: ";
            for (int coin : solution) {
                std::cout << coin << " ";
            }
            std::cout << "\n";
        }
    }
    
    // Count ways (ordered)
    long long waysOrdered = cc.countWaysOrdered(amount);
    std::cout << "Number of ways (ordered): " << waysOrdered << "\n";
    
    // Count ways (unordered - distinct combinations only)
    long long waysUnordered = cc.countWaysUnordered(amount);
    std::cout << "Number of ways (unordered): " << waysUnordered << "\n";
    
    return 0;
}
