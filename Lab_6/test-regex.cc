#include "testbase.hh"
#include "engine.hh"

#include <algorithm>
#include <cstdlib>
#include <iostream>


using namespace std;


/*===========================================================================
 * TEST FUNCTIONS
 *
 * These are called by the main() function at the end of this file.
 */


/*! Test simple character-match regex operations. */
void test_simple_regex(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("abc");
    Range r;

    ctx.DESC("Simple regex with find()");

    r = find(regex, "abc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "abcd");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "dabc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dabcd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Simple regex with match()");

    ctx.CHECK(match(regex, "abc"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "abcd"));
    ctx.CHECK(!match(regex, "dabc"));
    ctx.CHECK(!match(regex, "dabcd"));
    
    ctx.result();

    clearRegex(regex);
}


/*! Test wildcard-match operations. */
void test_simple_wildcards(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("a.c");
    Range r;

    ctx.DESC("Simple wildcard regex with find()");

    r = find(regex, "abc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "adc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "a!c");
    ctx.CHECK(r.start == 0 && r.end == 3);

    // Should find the first occurrence.
    r = find(regex, "aacc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "daqc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dazcd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Simple wildcard regex with match()");

    ctx.CHECK(match(regex, "abc"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "abcd"));
    ctx.CHECK(!match(regex, "dabc"));
    ctx.CHECK(!match(regex, "dabcd"));
    
    ctx.result();

    clearRegex(regex);
}


/*! Test character classes. */
void test_char_classes(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("a[aegi]c");
    Range r;

    ctx.DESC("Simple character-class regex with find()");

    r = find(regex, "aac");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "adc");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "aic");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "aaic");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dagc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "daecd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Simple character-class regex with match()");

    ctx.CHECK(match(regex, "aac"));
    ctx.CHECK(match(regex, "agc"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "aaic"));
    ctx.CHECK(!match(regex, "dagc"));
    ctx.CHECK(!match(regex, "daacd"));

    ctx.result();

    clearRegex(regex);
}


/*! Test inverted character classes. */
void test_inv_char_classes(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("a[^aegi]c");
    Range r;

    ctx.DESC("Inverted character-class regex with find()");

    r = find(regex, "abc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "agc");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "acc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "aabc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dafc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "damcd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Inverted character-class regex with match()");

    ctx.CHECK(match(regex, "abc"));
    ctx.CHECK(match(regex, "afc"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "aabc"));
    ctx.CHECK(!match(regex, "dafc"));
    ctx.CHECK(!match(regex, "damcd"));

    ctx.result();

    clearRegex(regex);
}


/*! Test the * repeat-modifier. */
void test_kleene_star(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("a.*c");
    Range r;

    ctx.DESC("Kleene star * with find()");

    r = find(regex, "ac");
    ctx.CHECK(r.start == 0 && r.end == 2);

    r = find(regex, "adc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "aasdfasdfasdfc");
    ctx.CHECK(r.start == 0 && r.end == 14);

    r = find(regex, "aasdfasdfasdfcghiqmlbarzqtdvy");
    ctx.CHECK(r.start == 0 && r.end == 14);

    r = find(regex, "ab");
    ctx.CHECK(r.start == -1 && r.end == -1);

    // Should consume all "c" characters.  The .* should consume "bc".
    r = find(regex, "abcc");
    ctx.CHECK(r.start == 0 && r.end == 4);

    r = find(regex, "dac");
    ctx.CHECK(r.start == 1 && r.end == 3);

    r = find(regex, "dafc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dacd");
    ctx.CHECK(r.start == 1 && r.end == 3);

    r = find(regex, "damcd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Kleene star * with match()");

    ctx.CHECK(match(regex, "abc"));
    ctx.CHECK(match(regex, "afc"));
    ctx.CHECK(match(regex, "ac"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "dac"));
    ctx.CHECK(!match(regex, "dafc"));
    ctx.CHECK(!match(regex, "damcd"));

    ctx.result();

    clearRegex(regex);
}


/*! Test the + repeat-modifier. */
void test_plus(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("a.+c");
    Range r;

    ctx.DESC("Plus + with find()");

    // Need at least one character separating a and c
    r = find(regex, "ac");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "adc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "aasdfasdfasdfc");
    ctx.CHECK(r.start == 0 && r.end == 14);

    r = find(regex, "aasdfasdfasdfcghiqmlbarzqtdvy");
    ctx.CHECK(r.start == 0 && r.end == 14);

    // Should consume all "c" characters.  The .* should consume middle "c".
    r = find(regex, "acc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    // Should consume all "c" characters.  The .* should consume "bc".
    r = find(regex, "abcc");
    ctx.CHECK(r.start == 0 && r.end == 4);

    r = find(regex, "dac");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "dafc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dacd");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "damcd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Plus + with match()");

    ctx.CHECK(match(regex, "abc"));
    ctx.CHECK(match(regex, "afc"));
    ctx.CHECK(!match(regex, "ac"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "daasdfasdfasdfc"));
    ctx.CHECK(!match(regex, "dafc"));
    ctx.CHECK(!match(regex, "damcd"));

    ctx.result();

    clearRegex(regex);
}


/*! Test the ? optional-modifier. */
void test_optional(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("ab?c");
    Range r;

    ctx.DESC("Optional operator ? with find()");

    r = find(regex, "ac");
    ctx.CHECK(r.start == 0 && r.end == 2);

    r = find(regex, "abc");
    ctx.CHECK(r.start == 0 && r.end == 3);

    r = find(regex, "adc");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "dac");
    ctx.CHECK(r.start == 1 && r.end == 3);

    r = find(regex, "dabc");
    ctx.CHECK(r.start == 1 && r.end == 4);

    r = find(regex, "dadc");
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "dacd");
    ctx.CHECK(r.start == 1 && r.end == 3);

    r = find(regex, "dabcd");
    ctx.CHECK(r.start == 1 && r.end == 4);
    
    ctx.result();

    ctx.DESC("Optional operator ? with match()");

    ctx.CHECK(match(regex, "ac"));
    ctx.CHECK(match(regex, "abc"));
    ctx.CHECK(!match(regex, "afc"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));
    
    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "dac"));
    ctx.CHECK(!match(regex, "dabc"));
    ctx.CHECK(!match(regex, "dacd"));
    ctx.CHECK(!match(regex, "dabcd"));

    ctx.result();

    clearRegex(regex);
}


/*! Test more complex regular expressions. */
void test_complex_regex(TestContext &ctx) {
    vector<RegexOperator *> regex = parseRegex("ab+c?d*[ef]+g[^ghi]*j.+k");
    Range r;

    ctx.DESC("Complex regular expression with find()");

    r = find(regex, "aegjkk");  // Missing "b"
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "abejkk");  // Missing "g"
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "abegijkk");  // Shouldn't have the "i"
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "abegkk");  // Missing "j"
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "abegjk");  // Missing letter between "j" and "k"
    ctx.CHECK(r.start == -1 && r.end == -1);

    r = find(regex, "abegjkk");
    ctx.CHECK(r.start == 0 && r.end == 7);

    r = find(regex, "abfgjkk");
    ctx.CHECK(r.start == 0 && r.end == 7);

    r = find(regex, "abegjjk");
    ctx.CHECK(r.start == 0 && r.end == 7);

    r = find(regex, "aaabegjkk");
    ctx.CHECK(r.start == 2 && r.end == 9);

    r = find(regex, "abegjkkmmmm");
    ctx.CHECK(r.start == 0 && r.end == 7);

    r = find(regex, "aaabegjkkmmmm");
    ctx.CHECK(r.start == 2 && r.end == 9);

    r = find(regex, "abbbbbbbbegjkk");
    ctx.CHECK(r.start == 0 && r.end == 14);

    r = find(regex, "aaabbbbbbbbegjkk");
    ctx.CHECK(r.start == 2 && r.end == 16);

    r = find(regex, "abegjkk");
    ctx.CHECK(r.start == 0 && r.end == 7);

    ctx.result();

    ctx.DESC("Complex regular expression with match()");

    ctx.CHECK(match(regex, "abegjkk"));
    ctx.CHECK(match(regex, "abbbbbbbbegjkk"));
    ctx.CHECK(!match(regex, "abegijkk"));

    ctx.CHECK(!match(regex, ""));
    ctx.CHECK(!match(regex, "a"));

    // These will successfully generate a "find", but it isn't a "match"
    // because it doesn't use the entire string.
    ctx.CHECK(!match(regex, "aaabegjkk"));
    ctx.CHECK(!match(regex, "aaabegjkkmmmm"));
    ctx.CHECK(!match(regex, "aaabbbbbbbbegjkk"));

    ctx.result();
    
    clearRegex(regex);
}


/*! This program is a simple test-suite for the Rational class. */
int main() {
  
    cout << "Testing regular expressions." << endl << endl;

    TestContext ctx(cout);

    test_simple_regex(ctx);
    test_simple_wildcards(ctx);
    test_char_classes(ctx);
    test_inv_char_classes(ctx);
    test_kleene_star(ctx);
    test_plus(ctx);
    test_optional(ctx);
    test_complex_regex(ctx);
    
    // Return 0 if everything passed, nonzero if something failed.
    return !ctx.ok();
}
