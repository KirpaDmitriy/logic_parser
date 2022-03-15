#include <string>
using namespace std;

struct Tree {
public:
    Tree* left;
    Tree* right;
    char op;
    string val;
    
    Tree(char operation, Tree* lchild, Tree* rchild) : op(operation),left(lchild),right(rchild),val("") {}
    Tree(string value) : val(value),op('V'),left(NULL),right(NULL) {}
    
    string print() {
        switch(op) {
            case '&': case '|':
                return '(' + left->print() + ' ' + op + ' ' + right->print() + ')';
            case '>':
                return '(' + left->print() + " -> " + right->print() + ')';
	    case '!':
		if(left->op != 'V') return "!(" + left->print() + ')';
		return '!' + left->print();
            case 'V':
                return val;
        }
    }
    
    int compare_with(Tree* other) {
        if(((op == other->op) && (op != 'V')) || ((op == other->op) && (op == 'V') && (val == other->val))) {
            if((left == NULL) && (other->left == NULL) && (right == NULL) && (other->right == NULL)) return 1;
            if((right == NULL) && (other->right == NULL) && (left != NULL) && (other->left != NULL)) return left->compare_with(other->left);
            if((right != NULL) && (other->right != NULL) && (left != NULL) && (other->left != NULL))return left->compare_with(other->left) && right->compare_with(other->right);
            return 0;
        }
        return 0;
    }
};
