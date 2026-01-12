#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <string>

const int ALPHABET_SIZE = 256;  // Support full ASCII range
const int MAX_STATES = 500000;

struct Aho {
    // Use flat array for better cache locality: next[state * ALPHABET_SIZE + char]
    std::vector<int> next;
    std::vector<int> out_link;  // out_link[state] -> next state with output (or -1)
    std::vector<int> pattern_idx;  // pattern_idx[state] -> pattern index if state ends a pattern (-1 otherwise)
    std::vector<int> failure;
    int states;
    
    std::vector<int> cnt;  // cnt[i] = number of times pattern i appears in input
    std::unordered_map<std::string, int> pattern_to_idx;  // Maps pattern string to index
    int next_idx = 0;
    
    Aho() : states(1) {
        next.resize(MAX_STATES * ALPHABET_SIZE, -1);
        out_link.resize(MAX_STATES, -1);
        pattern_idx.resize(MAX_STATES, -1);
        failure.resize(MAX_STATES, 0);
    }
    
    inline int& get_next(int state, int ch) {
        return next[state * ALPHABET_SIZE + ch];
    }
    
    void add_str(std::string& s) {
        if (pattern_to_idx.find(s) != pattern_to_idx.end()) {
            // Duplicate pattern - just increment count
            int idx = pattern_to_idx[s];
            cnt[idx]++;
            return;
        }
        
        // New pattern - add to trie
        int idx = next_idx++;
        pattern_to_idx[s] = idx;
        cnt.push_back(1);
        
        int state = 0;
        for (char c : s) {
            int ch = static_cast<unsigned char>(c);
            if (get_next(state, ch) == -1) {
                get_next(state, ch) = states++;
            }
            state = get_next(state, ch);
        }
        pattern_idx[state] = idx;
    }
    
    void gen_failure() {
        std::queue<int> q;
        
        // Set root failure to root
        failure[0] = 0;
        
        // Initialize root's children
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (get_next(0, i) != -1) {
                int child_state = get_next(0, i);
                q.push(child_state);
                failure[child_state] = 0;
            }
        }
        
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            
            // Process all transitions from current state
            for (int c = 0; c < ALPHABET_SIZE; c++) {
                if (get_next(curr, c) != -1) {
                    int node = get_next(curr, c);
                    q.push(node);
                    
                    // Find failure of current state that accepts char c
                    int nx = failure[curr];
                    while (nx != 0 && get_next(nx, c) == -1) {
                        nx = failure[nx];
                    }
                    if (get_next(nx, c) != -1) {
                        failure[node] = get_next(nx, c);
                    } else {
                        failure[node] = 0;
                    }
                    
                    // Set output link: link to first state in failure chain with output
                    if (pattern_idx[failure[node]] != -1 || out_link[failure[node]] != -1) {
                        out_link[node] = failure[node];
                    } else {
                        out_link[node] = out_link[failure[node]];
                    }
                }
            }
        }
    }
    
    std::vector<int> matches(std::string& s, const std::vector<std::string>& patterns) {
        // Get count of matches for each pattern in string s
        int state = 0;
        std::vector<int> match_counts(cnt.size(), 0);  // Match count per unique pattern
        
        for (int i = 0; i < s.size(); i++) {
            int ch = static_cast<unsigned char>(s[i]);
            
            // Keep falling back until a step can be taken or the root is reached
            while (state != 0 && get_next(state, ch) == -1) {
                state = failure[state];
            }
            
            // If a valid transition exists, take it and update answer based on outputs of that node
            if (get_next(state, ch) != -1) {
                state = get_next(state, ch);
                
                // Follow output chain
                int out_state = state;
                while (out_state != 0) {
                    if (pattern_idx[out_state] != -1) {
                        match_counts[pattern_idx[out_state]]++;
                    }
                    out_state = out_link[out_state];
                    if (out_state == -1) break;
                }
            }
        }
        
        // Map back to input patterns (all duplicates get the same match count)
        std::vector<int> ans(patterns.size());
        for (int i = 0; i < patterns.size(); i++) {
            int idx = pattern_to_idx[patterns[i]];
            ans[i] = match_counts[idx];
        }
        
        return ans;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    std::string target; 
    std::cin >> target;
    
    int cnt;
    std::cin >> cnt;
    std::vector<std::string> patterns;
    patterns.reserve(cnt);
    
    for (int i = 0; i < cnt; i++) {
        std::string s;
        std::cin >> s;
        patterns.push_back(s);
    }
    
    Aho aho;
    
    for (int i = 0; i < patterns.size(); i++) {
        aho.add_str(patterns[i]);
    }
    
    aho.gen_failure();
    
    std::vector<int> match_counts = aho.matches(target, patterns);
    
    for (int i = 0; i < patterns.size(); i++) {
        std::cout << match_counts[i] << "\n";
    }
}
