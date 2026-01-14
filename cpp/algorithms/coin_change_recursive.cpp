#include <vector>
#include <iostream>
#include <climits>
#include <algorithm>

// Recursive solution for coin change problem
// Problem: Given coins of different denominations and a total amount,
// find the minimum number of coins needed to make that amount

class CoinChange {
private:
    std::vector<int> coins;
    
    // Recursive function to find minimum coins needed
    // Returns minimum number of coins to make 'amount', or -1 if impossible
    int solveRecursive(int amount) {
        // Base cases
        if (amount == 0) {
            return 0;  // No coins needed for amount 0
        }
        if (amount < 0) {
            return -1;  // Invalid - negative amount
        }
        
        int minCoins = INT_MAX;
        
        // Try each coin
        for (int coin : coins) {
            int result = solveRecursive(amount - coin);
            if (result != -1) {
                minCoins = std::min(minCoins, result + 1);
            }
        }
        
        return (minCoins == INT_MAX) ? -1 : minCoins;
    }
    
    // Memoized version (top-down DP)
    std::vector<int> memo;
    
    int solveMemoized(int amount) {
        // Base cases
        if (amount == 0) {
            return 0;
        }
        if (amount < 0) {
            return -1;
        }
        
        // Check memo
        if (memo[amount] != -2) {
            return memo[amount];
        }
        
        int minCoins = INT_MAX;
        
        // Try each coin
        for (int coin : coins) {
            int result = solveMemoized(amount - coin);
            if (result != -1) {
                minCoins = std::min(minCoins, result + 1);
            }
        }
        
        memo[amount] = (minCoins == INT_MAX) ? -1 : minCoins;
        return memo[amount];
    }

public:
    CoinChange(std::vector<int>& coins) : coins(coins) {}
    
    // Pure recursive solution (can be very slow for large amounts)
    int coinChangeRecursive(int amount) {
        return solveRecursive(amount);
    }
    
    // Memoized recursive solution (top-down DP)
    int coinChangeMemoized(int amount) {
        memo.assign(amount + 1, -2);  // -2 means not computed yet
        return solveMemoized(amount);
    }
    
    // Count number of ways to make amount (recursive with memoization)
    std::vector<long long> waysMemo;
    
    long long countWays(int amount) {
        waysMemo.assign(amount + 1, -1);
        return countWaysRecursive(amount);
    }
    
private:
    long long countWaysRecursive(int amount) {
        if (amount == 0) {
            return 1;  // One way to make 0: use no coins
        }
        if (amount < 0) {
            return 0;
        }
        
        if (waysMemo[amount] != -1) {
            return waysMemo[amount];
        }
        
        long long ways = 0;
        for (int coin : coins) {
            ways += countWaysRecursive(amount - coin);
        }
        
        waysMemo[amount] = ways;
        return ways;
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
    
    // Use memoized version for efficiency
    int result = cc.coinChangeMemoized(amount);
    
    if (result == -1) {
        std::cout << "Cannot make amount with given coins\n";
    } else {
        std::cout << "Minimum coins needed: " << result << "\n";
    }
    
    // Optional: count number of ways
    long long ways = cc.countWays(amount);
    std::cout << "Number of ways to make amount: " << ways << "\n";
    
    return 0;
}
