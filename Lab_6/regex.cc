#include "regex.hh"
#include <iostream>


/* Initialize the regex operator to apply exactly once. */
RegexOperator::RegexOperator() {
    minRepeat = 1;
    maxRepeat = 1;
}


/* Returns the "minimum repeat count" value. */
int RegexOperator::getMinRepeat() const {
    return minRepeat;
}


/* Returns the "maximum repeat count" value. */
int RegexOperator::getMaxRepeat() const {
    return maxRepeat;
}


/* Sets the "minimum repeat count" value. */
void RegexOperator::setMinRepeat(int n) {
    assert(n >= 0);
    minRepeat = n;
}


/* Sets the "maximum repeat count" value. */
void RegexOperator::setMaxRepeat(int n) {
    assert(n >= -1);
    maxRepeat = n;
}


/* Clears the list of matches stored in the regex operator.  Typically done
 * in preparation to try to match the regex to a new string.
 */
void RegexOperator::clearMatches() {
    matches.clear();
}


/* Records a new match of the operator in the list of matches. */
void RegexOperator::pushMatch(const Range &r) {
    matches.push_back(r);
}


/* Reports how many times the regex operator has successfully matched in the
 * string.
 */
int RegexOperator::numMatches() const {
    return (int) matches.size();
}


/* Removes the last match the operator successfully matched against.  Used for
 * backtracking by the regex engine.
 */
Range RegexOperator::popMatch() {
    Range r = matches.back();
    matches.pop_back();
    return r;
}

bool MatchChar::match(const string &s, Range &r) const {
  if (r.start > (int) s.length() - 1) return false;
  if (s[r.start] == _c) {
    r.end = r.start + 1;
    return true;
  }
  return false;
}

bool MatchAny::match(const string &s, Range &r) const {
  if (r.start > (int) s.length() - 1) return false;
  if (!s.empty()) {
    r.end = r.start + 1;
    return true;
  }
  return false;
}

// Match any char in _s to s[r.start] and s[r.end-1]
bool MatchFromSubset::match(const string &s, Range &r) const {
  if (r.start > (int) s.length() - 1) return false;
  for (int i = 0; i < (int) _s.length(); i++) {
    if (s[r.start] == _s[i]) {
      r.end = r.start + 1;
      return true;
    }
  }
  return false;
}

bool ExcludeFromSubset::match(const string &s, Range &r) const {
  if (r.start > (int) s.length() - 1) return false;
  bool isNotMatch = false;
  for (int i = 0; i < (int) _s.length(); i++) {
    if (s[r.start] != _s[i]) isNotMatch = true;
    else {
      isNotMatch = false; 
      break;
    }
  }
  if (isNotMatch) r.end = r.start + 1;
  return isNotMatch;
}

// Iterate over characters in expr and find special characters
vector<RegexOperator *> parseRegex(const string &expr) {
  vector<RegexOperator *> vecOperators;
  int exprLen = (int) expr.length();
  for (int i = 0; i < exprLen; i++) {
    char c = expr[i];
    switch(c) {
      case '\\': { // Next char is literal
        char nextChr = expr[i+1];
        vecOperators.push_back(new MatchChar(nextChr));
        i++;
        break;
      }
      case '.': { // Replace with any char
        RegexOperator *regOp = new MatchAny();
        if (expr[i + 1] == '*') {
          regOp->setMinRepeat(0);
          regOp->setMaxRepeat(-1);
          i++;
        }
        else if (expr[i + 1] == '+') {
          regOp->setMaxRepeat(-1);
          i++;
        }
        vecOperators.push_back(regOp);
        break;
      }
      case '\?': { // 0 or 1 of prev char
        RegexOperator *regOp = new MatchChar(expr[i-1]);
        regOp->setMinRepeat(0);
        std::cout << "in regex.cc minrepeat: " << regOp->getMinRepeat() << std::endl;
        vecOperators.push_back(regOp);
        break;
      }
      case '*': { // 0 or more of prev char
          RegexOperator *regOp = new MatchChar(expr[i-1]);
          regOp->setMinRepeat(0);
          regOp->setMaxRepeat(-1);
          vecOperators.push_back(regOp);
          break;
      }
      case '+': { // 1 or more of prev char
        RegexOperator *regOp = new MatchChar(expr[i-1]);
        regOp->setMaxRepeat(-1);
        vecOperators.push_back(regOp);
        break;
      }
      case '[': { // Should also match [^] cases
        char nextChr = expr[++i];
        string matchStr;
        while (nextChr != ']') {
          matchStr.push_back(nextChr);
          nextChr = expr[++i];
        }
        if(matchStr[0] == '^') {
          string notMatchStr = matchStr.substr(1,string::npos);
          RegexOperator *regOp = new ExcludeFromSubset(notMatchStr);
          if (expr[i + 1] == '*') {
            regOp->setMinRepeat(0);
            regOp->setMaxRepeat(-1);
            i++;
          }
          else if (expr[i + 1] == '+') {
            regOp->setMaxRepeat(-1);
          }
          vecOperators.push_back(regOp);
        }
        else {
          RegexOperator *regOp = new MatchFromSubset(matchStr);
          if (expr[i + 1] == '*') {
            regOp->setMinRepeat(0);
            regOp->setMaxRepeat(-1);
            i++;
          }
          else if (expr[i + 1] == '+') {
            regOp->setMaxRepeat(-1);
            i++;
          }
          vecOperators.push_back(regOp);
        }
        break;
      }
      default: {
        char nextChr = expr[i + 1];
        if (nextChr != '\?' && nextChr != '*' && nextChr != '+') {
          RegexOperator *regOp = new MatchChar(expr[i]);
          vecOperators.push_back(regOp);
        }
      }
    }
  }
  return vecOperators;
}

void clearRegex(vector<RegexOperator *> regex) {
  for (int i = 0; i < (int) regex.size(); i++) delete regex[i];
}
