#include <vector>
#include <iostream>
#include <queue>

const int ALPHABET_SIZE = 26;
const int MAX_STATES = 500000;

struct Aho {
    // Use flat array for better cache locality: next[state * 26 + char]
    std::vector<int> next;
    std::vector<int> out_link;  // out_link[state] -> next state with output (or -1)
    std::vector<int> pattern_idx;  // pattern_idx[state] -> pattern index if state ends a pattern (-1 otherwise)
    std::vector<int> failure;
    int states;
    
    Aho() : states(1) {
        next.resize(MAX_STATES * ALPHABET_SIZE, -1);
        out_link.resize(MAX_STATES, -1);
        pattern_idx.resize(MAX_STATES, -1);
        failure.resize(MAX_STATES, 0);
    }
    
    inline int& get_next(int state, int ch) {
        return next[state * ALPHABET_SIZE + ch];
    }
    
    void add_str(std::string& s, int idx) {
        int state = 0;
        for (char c : s) {
            int ch = c - 'a';
            if (get_next(state, ch) == -1) {
                get_next(state, ch) = states++;
            }
            state = get_next(state, ch);
        }
        pattern_idx[state] = idx;  // Mark that this state ends pattern idx
    }
    
    void gen_failure() {
        std::queue<int> q;
        
        failure[0] = 0;
        
        // Initialize root's children and compute goto links
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (get_next(0, i) != -1) {
                q.push(get_next(0, i));
                failure[get_next(0, i)] = 0;
            } else {
                get_next(0, i) = 0;  // Goto link: point to root if no transition
            }
        }
        
        while (!q.empty()) {
            int curr = q.front();
            q.pop();
            
            for (int c = 0; c < ALPHABET_SIZE; c++) {
                if (get_next(curr, c) == -1) {
                    // Compute goto link: follow failure chain
                    get_next(curr, c) = get_next(failure[curr], c);
                } else {
                    int node = get_next(curr, c);
                    q.push(node);
                    
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
                    if (pattern_idx[failure[node]] != -1) {
                        out_link[node] = failure[node];
                    } else {
                        out_link[node] = out_link[failure[node]];
                    }
                }
            }
        }
    }
    
    std::vector<int> matches(std::string& s, int num_patterns) {
        int state = 0;
        std::vector<int> ans(num_patterns, 0);
        
        for (char c : s) {
            int ch = c - 'a';
            state = get_next(state, ch);  // No while loop needed - goto links handle it
            
            // Follow output chain (check current state first, then follow links)
            int out_state = state;
            while (out_state != 0) {
                if (pattern_idx[out_state] != -1) {
                    ans[pattern_idx[out_state]]++;
                }
                out_state = out_link[out_state];
                if (out_state == -1) break;
            }
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
        aho.add_str(patterns[i], i);
    }
    
    aho.gen_failure();
    
    std::vector<int> matches = aho.matches(target, cnt);
    
    for (int i = 0; i < cnt; i++) {
        std::cout << matches[i] << "\n";
    }
}
