class Solution {
public:
    int missingNumber(vector<int>& nums) {
        // Get the size of input array
        int n = nums.size();
        
        // Will store XOR of all numbers from 0 to n
        int Xorall = 0;
        
        // Will store XOR of all numbers in input array
        int XorNums = 0;
        
        // Calculate XOR of all numbers from 0 to n
        // Example: if n = 3, calculates 0^1^2^3
        for(int i = 0; i <= n; i++) {
            Xorall ^= i;
        }
        
        // Calculate XOR of all numbers in input array
        // Example: if nums = [0,1,3], calculates 0^1^3
        for(auto i : nums) {
            XorNums ^= i;
        }
        
        // XOR of Xorall and XorNums gives the missing number
        // Because:
        // 1. XOR of a number with itself is 0
        // 2. XOR is associative and commutative
        // 3. All numbers except the missing one will XOR to 0
        // 4. Only the missing number will remain
        return Xorall ^ XorNums;
    }
};