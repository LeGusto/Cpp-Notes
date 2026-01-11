#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <queue>

struct Node {
    std::unordered_map<char, Node> next;
    std::vector<int> out;
    Node* failure = nullptr;
};

struct Aho {
    Node root;
    std::vector<std::string> patterns;

    void add_str(std::string& s) {
        Node* curr = &root;
        for (int i = 0; i < s.size(); i++) {
            if (curr->next.find(s[i]) == curr->next.end()) {
                curr->next[s[i]] = Node();
            }
            curr = &curr->next[s[i]];
        }
        curr->out.push_back(patterns.size());
        patterns.push_back(s);
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

                // Merge output from failure state
                for (auto &v : node.failure->out) {
                    node.out.push_back(v);
                }
            }
 
        }
    }

    std::vector<int> matches(std::string& s) {
        // Get count of matches for each pattern in string s
        Node* curr = &root;
        std::vector<int> ans(patterns.size(), 0);

        for (int i = 0; i < s.size(); i++) {
            // Keep falling back until a step can be taken or the root is reached
            while (curr != &root && curr->next.find(s[i]) == curr->next.end()) {
                curr = curr->failure;
            }

            // If a valid transition exists, take it and update answer based on outputs of that node
            if (curr->next.find(s[i]) != curr->next.end()) {
                curr = &curr->next[s[i]];
                for (auto &idx : curr->out) {
                    ans[idx]++;
                }
            }
        }

        return ans;
    }
};


int main() {
    std::string target; 
    std::cin>>target; //= "abaacbcabc";
    std::string p;
    std::cin>>p;
    std::vector<std::string> patterns = {p};
    Aho aho {};

    for (auto &v : patterns) aho.add_str(v);
    
    aho.gen_failure();
    
    std::vector<int> matches = aho.matches(target);

    for (int i = 0; i < patterns.size(); i++) {
        std::cout<<matches[i]<<"\n";
    }
}