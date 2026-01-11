#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <string>

struct Node {
    std::unordered_map<char, Node> next;
    int pattern_idx = -1;  // Index of pattern that ends here (-1 if none)
    Node* out_link = nullptr;  // Link to next state with output in failure chain
    Node* failure = nullptr;
};

struct Aho {
    Node root;
    std::vector<int> cnt;  // cnt[i] = number of times pattern i appears in input
    std::unordered_map<std::string, int> pattern_to_idx;  // Maps pattern string to index
    int next_idx = 0;

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
        
        Node* curr = &root;
        for (int i = 0; i < s.size(); i++) {
            if (curr->next.find(s[i]) == curr->next.end()) {
                curr->next[s[i]] = Node();
            }
            curr = &curr->next[s[i]];
        }
        curr->pattern_idx = idx;
    }

    void gen_failure() {
        std::queue<Node*> q;

        // Set root and immediate children failure to root
        root.failure = &root;

        for (auto &[c, node] : root.next) {
            // Insert children for processing
            q.push(&node);
            node.failure = &root;
        }

        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();

            // Get all valid transitions
            for (auto &[c, node] : curr->next) {
                // Push for processing later
                q.push(&node);
                
                // Find failure of current state that accepts char c
                Node* nx = curr->failure;
                while (nx != &root && nx->next.find(c) == nx->next.end()) {
                    nx = nx->failure;
                }
                if (nx->next.find(c) != nx->next.end()) {
                    node.failure = &nx->next[c];
                } else {
                    node.failure = &root;
                }

                // Set output link: link to first state in failure chain with output
                if (node.failure->pattern_idx != -1 || node.failure->out_link != nullptr) {
                    node.out_link = node.failure;
                } else {
                    node.out_link = node.failure->out_link;
                }
            }
 
        }
    }

    std::vector<int> matches(std::string& s, const std::vector<std::string>& patterns) {
        // Get count of matches for each pattern in string s
        Node* curr = &root;
        std::vector<int> match_counts(cnt.size(), 0);  // Match count per unique pattern

        for (int i = 0; i < s.size(); i++) {
            // Keep falling back until a step can be taken or the root is reached
            while (curr != &root && curr->next.find(s[i]) == curr->next.end()) {
                curr = curr->failure;
            }

            // If a valid transition exists, take it and update answer based on outputs of that node
            if (curr->next.find(s[i]) != curr->next.end()) {
                curr = &curr->next[s[i]];
                
                // Follow output chain
                Node* out_node = curr;
                while (out_node != nullptr) {
                    if (out_node->pattern_idx != -1) {
                        match_counts[out_node->pattern_idx]++;
                    }
                    out_node = out_node->out_link;
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
    std::cin>>target; //= "abaacbcabc";

    int cnt;
    std::cin>>cnt;
    std::vector<std::string> patterns;
    patterns.reserve(cnt);

    for (int i = 0; i < cnt; i++) {
        std::string s;
        std::cin>>s;
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