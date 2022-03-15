#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include "expression.h"
#include "parser/expression.tab.h"
#include "parser/expression.lexer.h"
#include <vector>
#include <hash_map>
using namespace std;

extern Tree* result;

void yyerror(const char *error) {
    cerr << error;
}

int yywrap() {
    return 1;
}

void axiom(const string* alpha, const string* prv) {
    cout << '(' + *prv + "->(" + *alpha + "->" + *prv + "))" << endl;
    cout << '(' + *prv + ')' << endl;
    cout << '(' + *alpha + "->" + *prv + ')';
}

void alph(const string* a) {
    cout << '(' + *a + "->(" + *a + "->" + *a + "))" << endl;
    cout << "((" + *a + "->(" + *a + "->" + *a + "))->((" + *a + "->(" + *a + "->" + *a + ")->" + *a + ")->(" + *a + "->" + *a + ")))" << endl;
    cout << "((" + *a + "->(" + *a + "->" + *a + ")->" + *a + ")->(" + *a + "->" + *a + "))" << endl;
    cout << '(' + *a + "->(" + *a + "->" + *a + ")->" + *a + ')' << endl;
    cout << '(' + *a + "->" + *a + ')';
}

void mopo(const string* a, const string* di, const string* dj) {
    cout << "((" + *a + "->" + *dj + ")->((" + *a + "->" + *dj + "->" + *di + ")->(" + *a + "->" + *di + ")))" << endl;
    cout << "((" + *a + "->" + *dj + "->" + *di + ")->(" + *a + "->" + *di + "))" << endl;
    cout << '(' + *a + "->" + *di + ')';
}

int main() {
    string first_line, hyps_txt = "", tmp_tree = "";
    getline(cin, first_line);
    Tree alpha = *(new Tree("NOTHING"));
    int i = 0;
    vector<Tree> hyps;
    for(; i<first_line.size(); i++) {
	if(first_line[i] == ',') { 
	    hyps_txt += tmp_tree + ',';
	    yy_scan_string(tmp_tree.c_str());
	    yyparse();
	    hyps.push_back(*result);
	    tmp_tree = "";
	}
	else {
	    if(first_line[i] == '|')
		if(i < first_line.size() - 1)
		    if(first_line[i + 1] == '-') {
	    		yy_scan_string(tmp_tree.c_str());
	    		yyparse();
	    		alpha = *result;
			break;
		    }
            tmp_tree += first_line[i];
	}
    }
    tmp_tree = '(' + alpha.print() + ')';
    string tmp_beta;
    i += 2;
    for(; i<first_line.size(); i++) {
	tmp_beta += first_line[i];
    }
    first_line = "";
    yy_scan_string(tmp_beta.c_str());
    yyparse();
    Tree beta = *result;
    string header = (new Tree('>', &alpha, &beta))->print();
    cout << hyps_txt.substr(0, hyps_txt.size() - 1) << "|- " << header << endl;
    hyps_txt = ""; header = "";
    vector<Tree> already_proved;
    vector<Tree> mps;
    vector<string> mpsh;
    for(;;) {
	string tmp_step;
	getline(cin, tmp_step);
	yy_scan_string(tmp_step.c_str());
	yyparse();
	Tree step = *result;
	tmp_step = '(' + tmp_step + ')';
        if(step.compare_with(&alpha) == 1) {
	    alph(&tmp_tree);
	    cout << endl;
        }
	else {
            int type=0;
            for(int hps = 0; hps < hyps.size(); hps++) {
	        if(step.compare_with(&hyps[hps]) == 1) {
		    type = -1; break;
		}
	    }
	  
	    if(type != -1) {
	    
	    if(step.op == '>') {
		    if(step.right->left) if(step.compare_with(new Tree('>', step.left, new Tree('>', step.right->left, step.left))) == 1) { type = -1; }  // ax 1
		    if(type != -1) if(step.left->left && step.left->right && step.right->right) if(step.right->right->right) if(step.compare_with(new Tree('>', new Tree('>', step.left->left, step.left->right), new Tree('>', new Tree('>', step.left->left, new Tree('>', step.left->right, step.right->right->right)), new Tree('>', step.left->left, step.right->right->right)))) == 1) { type = -1; }
		    if(type != -1) if(step.right->left) if(step.compare_with(new Tree('>', step.left, new Tree('>', step.right->left, new Tree('&', step.left, step.right->left))))) { type = -1; } // ax 3
		    if(type != -1) if(step.left->right) if(step.compare_with(new Tree('>', new Tree('&', step.right, step.left->right), step.right)) == 1) { type = -1; } // ax 4
		    if(type != -1) if(step.left->left) if(step.compare_with(new Tree('>', new Tree('&', step.left->left, step.right), step.right)) == 1) { type = -1; } // ax 5
		    if(type != -1) if(step.right->right) if(step.compare_with(new Tree('>', step.left, new Tree('|', step.left, step.right->right))) == 1) { type = -1; } // ax 6
		    if(type != -1) if(step.right->left) if(step.compare_with(new Tree('>', step.left, new Tree('|', step.right->left, step.left))) == 1) { type = -1; } // ax 7
		    if(type != -1) if(step.left->left && step.left->right && step.right->left) if(step.right->left->left) if(step.compare_with(new Tree('>', new Tree('>', step.left->left, step.left->right), new Tree('>', new Tree('>', step.right->left->left, step.left->right), new Tree('>', new Tree('|', step.left->left, step.right->left->left), step.left->right)))) == 1) { type = -1; }
		    if(type != -1) if(step.left->left && step.left->right) if(step.compare_with(new Tree('>', new Tree('>', step.left->left, step.left->right), new Tree('>', new Tree('>', step.left->left, new Tree('!', step.left->right, NULL)), new Tree('!', step.left->left, NULL)))) == 1) { type = -1; }
		    if(type != -1) if(step.compare_with(new Tree('>', new Tree('!', new Tree('!', step.right, NULL), NULL), step.right)) == 1) { type = -1; } // ax 10
	        }
	        if(type != -1) type = 1;
	    }
	    
	    if(type == 1) {
	    Tree* dj = NULL;
	    string stepstr = step.print();
	    for(int j=0; j<mps.size(); j++) {
	    	if(step.compare_with(mps[j].right) == 1) { dj = mps[j].left; break; }
	    }
	        string djstr = '(' + dj->print() + ')';
		mopo(&tmp_tree, &tmp_step, &djstr);
		cout << endl;
            }
	    else {
		axiom(&tmp_tree, &tmp_step);
	    	cout << endl;
            }
	}

	if(step.compare_with(&beta) == 1) break;
	for(int k=0; k<already_proved.size(); k++) {
		if(step.op == '>') if(already_proved[k].compare_with(step.left) == 1) { mps.push_back(step); }
		if(already_proved[k].op == '>') if(already_proved[k].left->compare_with(&step) == 1) { mps.push_back(already_proved[k]); }
		}
		already_proved.push_back(step);
    }
    return 0;
}
