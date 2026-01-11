#include <iostream>
#include <vector>

int main() {
    std::string s;
    std::cin>>s;
    int cnt;
    std::cin>>cnt;
    std::vector<std::string> patterns;

    for (int i = 0; i < cnt; i++) {
        std::string s; 
        std::cin>>s; 
        patterns.push_back(s);
    }

    int n = s.size();

    std::vector<int> LPS;
    
    for (auto &goal : patterns) {
        int m = goal.size();
        LPS.assign(m, 0);

        int len = 0;

        for (int i = 1; i < m; i++) {
            while (len > 0 && goal[i] != goal[len]) {
                len = LPS[len - 1];
            }
            if (goal[i] == goal[len]) {
                LPS[i] = len + 1;
                len++;
            }
        }

        int LPS_p = 0;
        int ans = 0;

        for (int i = 0; i < n; i++) {
            while (LPS_p > 0 && goal[LPS_p] != s[i]) LPS_p = LPS[LPS_p - 1];
            if (s[i] == goal[LPS_p]) {
                if (LPS_p < m - 1) {
                    LPS_p++;
                } else {
                    LPS_p = LPS[LPS_p]; 
                    ans++;
                }
            } 
        }

        std::cout<<ans<<"\n";
    }
    
}