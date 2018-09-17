/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2018, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Teigha(R) software pursuant to a license 
//   agreement with Open Design Alliance.
//   Teigha(R) Copyright (C) 2002-2018 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#include <time.h>

#include "OdaCommon.h"
#include "OdUnitsFormatter.h"

#define STL_USING_MAP
#define STL_USING_SET
#define STL_USING_STREAM
#define STL_USING_MEMORY
#include "OdaSTL.h"
#include <cstdlib>
#include <cwctype>
#include <string>

#include "OdExpression.h"

namespace OdExpression {

  typedef std::set<OdString> OdStringSet;
  typedef std::map<OdString, OdString> OdStringMap;
  
  enum Type {
    NONE, NUMBER, NAME, VARIABLE, UNARY_MINUS, RANDOM, FUNCTION, FUNCTION2, //
    LPAREN, // '('
    COMMA,  // ','
    RPAREN, // ')'
    OPERATOR = 0x20, // operator mask
    OP_PLUS,  // '+'
    OP_MINUS, // '-'
    OP_MOD,   // '%'
    OP_MULT,  // '*'
    OP_DIV,   // '/'
    OP_POW,   // '^'
    END
};

struct Term {
  typedef OdSharedPtr<Term> ptr;

  static ptr create(double value);
  static ptr create(Type type, Term::ptr arg0 = 0, Term::ptr arg1 = 0);
  static ptr create(const OdString& name, Term::ptr arg0 = 0, Term::ptr arg1 = 0);

  Term() : type(NONE) {}

  Type type;
  double value;
  OdString name;
  Term::ptr arg[2];
};

Term::ptr Term::create(double value)
{
  Term::ptr res(new Term());
  res->type = NUMBER;
  res->value = value;
  return res;
}

Term::ptr Term::create(Type type, Term::ptr arg0, Term::ptr arg1)
{
  Term::ptr res(new Term());
  res->type = type;
  res->arg[0] = arg0;
  res->arg[1] = arg1;
  return res;
}

Term::ptr Term::create(const OdString& name, Term::ptr arg0, Term::ptr arg1)
{
  Term::ptr res(new Term());
  res->type = (!arg0) ? VARIABLE : ((!arg1) ? FUNCTION : FUNCTION2);
  res->name = name;
  res->arg[0] = arg0;
  res->arg[1] = arg1;
  return res;
}

class ImpParser {
public:
  ImpParser();

  OdResult setName(OdString name);
  OdResult addVariable(OdString name, double value);
  OdResult fixVariable(OdString name, double value);
  OdResult renameVariable(OdString oldName, const OdString& newName);

  OdResult setExpression(const OdString& expression);

  bool isConstExpression();
  double evaluate() { return eval(m_root); }
  OdResult getVariables(OdStringArray& usedVars);

  OdString getTranslated() { return m_translated; }

private:
  double eval(Term* term);
  bool isConst(Term* term);
  void getVars(Term* term, OdStringSet& usedVars);

  Term::ptr parseExpr();
  Term::ptr parseMult();
  Term::ptr parsePow();
  Term::ptr parseTerm();

  void getToken(bool ignoreSign = true);
  void check(Type token);
  void error(const OdString& msg);

  typedef bool(*CharCheck)(OdChar c);

  OdChar nextChar() { m_translated += m_pChar[0]; return *++m_pChar; }
  OdString getWord() { return OdString(m_pWord, m_pChar - m_pWord); }
  OdChar skip(CharCheck f) { OdChar c = *m_pChar; while (f(c)) c = nextChar(); return c; }
  bool peek(CharCheck f) { bool res = f(*m_pChar); if (res) nextChar(); return res; }
  bool store(const OdChar* &pChar, CharCheck f);
  void revert(const OdChar* pWord);

  OdString m_name;
  OdString m_expr;
  OdString m_translated;
  const OdChar* m_pChar;
  const OdChar* m_pWord;
  Term m_current;
  Term::ptr m_root;
  std::map<OdString, double> m_variables;
  OdStringMap m_mapping;
  OdStringSet m_fixed_variables;
};

#pragma MARKMESSAGE("!!! Temporary fix for statically linked runtime on VC6-VC2003");

#if defined(_MSC_VER) && (_MSC_VER < 1400) && !defined(_DLL) && !defined (_WIN32_WCE)
static wctype_t sAlNum = wctype("alnum");

#define iswalnum(c)  iswctype(c, sAlNum)
#endif

static bool is_signum(OdChar c)   { return c == '+' || c == '-'; }
static bool is_minus(OdChar c)    { return c == '-'; }
static bool is_fraction(OdChar c) { return c == '/'; }
static bool is_spacer(OdChar c)   { return c == '-' || c == ' '; }
static bool is_exponent(OdChar c) { return c == 'e' || c == 'E'; }
static bool is_dot(OdChar c)      { return c == '.'; }
static bool is_char(OdChar c)     { return (unsigned)(c + 1) <= 256; }
static bool is_num(OdChar c)      { return c == '.' || (is_char(c) && isdigit(c)); }
static bool is_digit(OdChar c)    { return is_char(c) && !!isdigit(c); }
static bool is_space(OdChar c)    { return is_char(c) && !!isspace(c); }
static bool is_word(OdChar c)     { return c == '_' || iswalnum(c); }
static bool is_foot(OdChar c)     { return c == '\''; }
static bool is_inch(OdChar c)     { return c == '"'; }

static double square(double x){ return x * x; }
static double sign(double x)  { return (x > 0.0) ? 1.0 : ((x < 0.0) ? -1.0 : 0.0); }
static double acosh(double x) { return log(x + sqrt(square(x) - 1.0)); }
static double asinh(double x) { return sign(x) * log(fabs(x) + sqrt(square(x) + 1.0)); }
static double atanh(double x) { return 0.5 * (log(x + 1.0) - log(1.0 - x)); }
static double round(double x) { return int(floor(x + 0.5)); }
static double trunc(double x) { return int(x); }
static double d2r(double x)   { return OdaToRadian(x); }
static double r2d(double x)   { return OdaToDegree(x); }
static double exp10(double x) { return pow(10.0, x); }

static double cosd(double x)  { return cos(d2r(x)); }
static double sind(double x)  { return sin(d2r(x)); }
static double tand(double x)  { return tan(d2r(x)); }
static double acosd(double x) { return r2d(acos(x)); }
static double asind(double x) { return r2d(asin(x)); }
static double atand(double x) { return r2d(atan(x)); }

static double doMod(double x, double y) { return int(floor(x + 0.5)) % int(floor(y + 0.5)); }
static double doMax(double x, double y) { return (x > y) ? x : y; }
static double doMin(double x, double y) { return (x < y) ? x : y; }

static double doRand() { return double(std::rand()) / (RAND_MAX + 1.0); }
static bool set_rand() { std::srand((unsigned)time(NULL)); return true; }

struct Functions {
  bool isUnary(const OdString& name) { return m_unary.find(name) != m_unary.end(); }
  bool isBinary(const OdString& name) { return m_binary.find(name) != m_binary.end(); }

  typedef double (*Unary)(double arg);
  typedef double (*Binary)(double arg0, double arg1);
  std::map<OdString, Unary> m_unary;
  std::map<OdString, Binary> m_binary;
  Functions();
};

static Functions& functions()
{
  static Functions functions;
  return functions;
}
#define s_functions functions()

#define ADD_FUNCTION_ALIAS(f, a)  m_unary[OD_T(#f)]  = a
#define ADD_FUNCTION_ALIAS2(f, a) m_binary[OD_T(#f)] = a
#define ADD_FUNCTION(f)     ADD_FUNCTION_ALIAS(f, f)
#define ADD_FUNCTION_DEG(f) ADD_FUNCTION_ALIAS(f, f##d)
#define ADD_FUNCTION2(f)    ADD_FUNCTION_ALIAS2(f, f)

Functions::Functions()
{
  ADD_FUNCTION_DEG(cos);
  ADD_FUNCTION_DEG(sin);
  ADD_FUNCTION_DEG(tan);
  ADD_FUNCTION_DEG(acos);
  ADD_FUNCTION_DEG(asin);
  ADD_FUNCTION_DEG(atan);
  ADD_FUNCTION(cosh);
  ADD_FUNCTION(sinh);
  ADD_FUNCTION(tanh);
  ADD_FUNCTION(acosh);
  ADD_FUNCTION(asinh);
  ADD_FUNCTION(atanh);
  ADD_FUNCTION(sqrt);
  ADD_FUNCTION(sign);
  ADD_FUNCTION(round);
  ADD_FUNCTION(trunc);
  ADD_FUNCTION(floor);
  ADD_FUNCTION(ceil);
  ADD_FUNCTION(d2r);
  ADD_FUNCTION(r2d);
  ADD_FUNCTION(exp);
  ADD_FUNCTION(exp10);
  ADD_FUNCTION2(pow);
  ADD_FUNCTION_ALIAS(abs, fabs);
  ADD_FUNCTION_ALIAS(ln, log);
  ADD_FUNCTION_ALIAS(log, log10);
  ADD_FUNCTION_ALIAS2(max, doMax);
  ADD_FUNCTION_ALIAS2(min, doMin);
}

static const OdConstString s_random  = OD_T("random");
static const OdConstString s_constPi = OD_T("pi");
static const OdConstString s_constE  = OD_T("e");
static bool s_rand_initialized = set_rand();

ImpParser::ImpParser()
{
  addVariable(s_constPi, OdaPI);
  addVariable(s_constE,  2.71828182845904523536);
}

OdResult ImpParser::setName(OdString name)
{
  name.makeLower();
  if (name.isEmpty() || is_digit(name[0]))
    return eInvalidInput;
  for (int i = 0; i < name.getLength(); i++) {
    if (!is_word(name[i]))
      return eInvalidInput;
  }
  if (name == s_random || m_variables.find(name) != m_variables.end())
    return eDuplicateKey;
  m_name = name;
  return eOk;
}

OdResult ImpParser::fixVariable(OdString name, double value)
{
  OdResult res = addVariable(name, value);
  if (res == eOk)
    m_fixed_variables.insert(name.makeLower());
  return res;
}

OdResult ImpParser::renameVariable(OdString oldName, const OdString& newName)
{
  m_mapping[oldName.makeLower()] = newName;
  return eOk;
}

OdResult ImpParser::addVariable(OdString name, double value)
{
  name.makeLower();
  if (name == m_name)
    return eDuplicateKey;
  m_variables[name] = value;
  m_mapping[name] = name;
  return eOk;
}

double ImpParser::eval(Term* e)
{
  if (e) {
    switch (e->type) {
      case NUMBER:      return e->value;
      case VARIABLE:    return m_variables[e->name];
      case UNARY_MINUS: return -eval(e->arg[0]);
      case RANDOM:      return doRand();
      case OP_MOD:      return doMod(eval(e->arg[0]), eval(e->arg[1]));
      case OP_PLUS:     return eval(e->arg[0]) + eval(e->arg[1]);
      case OP_MINUS:    return eval(e->arg[0]) - eval(e->arg[1]);
      case OP_MULT:     return eval(e->arg[0]) * eval(e->arg[1]);
      case OP_DIV:      return eval(e->arg[0]) / eval(e->arg[1]);
      case OP_POW:      return pow(eval(e->arg[0]), eval(e->arg[1]));
      case FUNCTION:    return s_functions.m_unary[e->name](eval(e->arg[0]));
      case FUNCTION2:   return s_functions.m_binary[e->name](eval(e->arg[0]), eval(e->arg[1]));
      default: error(OD_T("Can not evaluate expression")); break;
    }
  }
  return 0.0;
}

OdResult ImpParser::setExpression(const OdString& expression)
{
  m_current.type = NONE;
  m_expr = expression;
  m_pChar = m_expr.c_str();
  try {
    m_root = parseExpr();
    if (m_current.type != END)
      error(OD_T("Unexpected end of expression"));
  }
  catch (...) {
    m_root = 0;
    return eInvalidInput;
  }
  return eOk;
}

Term::ptr ImpParser::parseExpr()
{
  Term::ptr res(parseMult()), term;
  for (;;) {
    Type type = m_current.type;
    switch (type) {
      case OP_PLUS:
      case OP_MINUS:
        term = parseMult();
        res = Term::create(type, res, term);
        break;
      default:
        return res;
    }
  }
}

Term::ptr ImpParser::parseMult()
{
  Term::ptr res(parsePow()), term;
  for (;;) {
    Type type = m_current.type;
    switch (type) {
      case OP_MULT:
      case OP_DIV:
      case OP_MOD:
        term = parsePow();
        res = Term::create(type, res, term);
        break;
      default:
        return res;
    }
  }
}

Term::ptr ImpParser::parsePow()
{
  Term::ptr res(parseTerm()), term;
  for (;;) {
    Type type = m_current.type;
    switch (type) {
      case OP_POW:
        term = parseTerm();
        res = Term::create(type, res, term);
        break;
      default:
        return res;
    }
  }
}

Term::ptr ImpParser::parseTerm()
{
  getToken(false);

  Term::ptr term;
  OdString word = m_current.name;
  word.makeLower();
  switch (m_current.type) {
    case NUMBER:
      term = Term::create(m_current.value);
      getToken();
      break;
    case NAME:
      {
        int transEnd = m_translated.getLength() - word.getLength();
        const OdChar* pToken = m_pChar;
        getToken();
        if (m_current.type == LPAREN) {
          if (word == s_random) {
            getToken();
            check(RPAREN);
            getToken();
            term = Term::create(RANDOM);
            break;
          }
          if (s_functions.isUnary(word)) {
            term = parseExpr();
            check(RPAREN);
            getToken();
            term = Term::create(word, term);
            break;
          }
          if (s_functions.isBinary(word)) {
            term = parseExpr();
            check(COMMA);
            Term::ptr term2(parseExpr());
            check(RPAREN);
            getToken();
            term = Term::create(word, term, term2);
            break;
          }
          error(OdString(OD_T("Function not found: ")) + word);
          break;
        }
        if (word == s_random) {
          term = Term::create(RANDOM);
          break;
        }
        if (m_fixed_variables.find(word) != m_fixed_variables.end()) {
          m_translated.deleteChars(transEnd, m_translated.getLength() - transEnd);
          m_translated += OdString().format(OD_T("%g"), m_variables[word]);
          m_translated += OdString(pToken, m_pChar - pToken);
          term = Term::create(m_variables[word]);
          break;
        } else if (!m_mapping[word].isEmpty() && word != m_mapping[word]) {
          word = m_mapping[word];
          m_translated.deleteChars(transEnd, m_translated.getLength() - transEnd);
          m_translated += word;
          m_translated += OdString(pToken, m_pChar - pToken);
        }
        term = Term::create(word);
        break;
      }
    case OP_MINUS:
      term = parseExpr();
      term = Term::create(UNARY_MINUS, term);
      break;
    case OP_PLUS:
      term = parseExpr();
      break;
    case LPAREN:
      term = parseExpr();
      check(RPAREN);
      getToken();
      break;
    default:
      error(OdString(OD_T("Unexpected: ")) + m_pWord);
      break;
  }
  return term;
}

void ImpParser::getToken(bool ignoreSign)
{
  Type& type = m_current.type;
  m_current.name.empty();
  OdChar firstChar = skip(is_space);

  m_pWord = m_pChar;

  if (!firstChar) {
    if (type == END)
      error(OD_T("Unexpected end of expression"));
    type = END;
    return;
  }

  OdChar secondChar = m_pWord[1];
  if (!ignoreSign && is_signum(firstChar)) {
    nextChar();
    secondChar = skip(is_space);
    if (is_num(secondChar))
      m_pWord = m_pChar;
    else
      revert(m_pWord);
  }

  if ((!ignoreSign && is_signum(firstChar) && is_num(secondChar)) ||
      is_digit(firstChar) ||
      (is_dot(firstChar) && is_digit(secondChar)))
  {
    skip(is_num);
    if (peek(is_exponent)) {
      peek(is_signum);
      skip(is_digit);
    }
    bool fraction = false;
    const OdChar* pBound = 0;
    if (!peek(is_inch)) { // skip Architectural chars, logic taken form unformatL()
      if (peek(is_foot)) {
        peek(is_spacer); store(pBound, is_minus);
        skip(is_num);
        if (peek(is_fraction) && store(pBound, is_fraction))
          skip(is_digit);
        else if (peek(is_spacer) && store(pBound, is_minus)) {
          skip(is_digit);
          if (peek(is_fraction))
            skip(is_digit);
        }
      }
      else if (peek(is_fraction) && store(pBound, is_fraction)) {
        skip(is_digit);
        fraction = true;
      }
      else if (peek(is_spacer) && store(pBound, is_minus)) {
        skip(is_digit);
        if (peek(is_fraction))
          skip(is_digit);
      }
      if (peek(is_inch))
        fraction = false;
    }
    for (;;) {
      OdString num = getWord();
      try {
        m_current.value = OdUnitsFormatter::unformatL(num.trimRight());
      }
      catch (...) {
        if (pBound) {
          revert(pBound); // do not consider '-' or '/' as fraction separator for the second time
          pBound = 0;
          continue;
        }
        else {
          error(OdString(OD_T("Bad number: ")) + num);
          return;
        }
      }
      break;
    }
    if (fraction && peek(is_foot)) // 1/2' was not supported
      m_current.value *= 12;
    if (is_minus(firstChar))
      m_current.value = -m_current.value;
    type = NUMBER;
    return;
  }

  switch (firstChar) {
    case '+': type = (type & OPERATOR) ? NONE : OP_PLUS; break;
    case '-': type = (type & OPERATOR) ? NONE : OP_MINUS; break;
    case '%': type = OP_MOD;   break;
    case '*': type = OP_MULT;  break;
    case '/': type = OP_DIV;   break;
    case '^': type = OP_POW;   break;
    case '(': type = LPAREN;   break;
    case ',': type = COMMA;    break;
    case ')': type = RPAREN;   break;
    default:   type = NONE;     break;
  }
  if (type != NONE) {
    nextChar();
    m_current.name = getWord();
    return;
  }

  if (!is_word(firstChar)) {
    error(OdString(OD_T("Unexpected character: ")) + firstChar);
    return;
  }

  skip(is_word);
  type = NAME;
  m_current.name = getWord();
}

void ImpParser::check(Type token)
{
  if (m_current.type != token)
    error(OdString(OD_T("Wrong sequence: ")) + m_pWord);
}

void ImpParser::error(const OdString& msg)
{
  throw OdError(msg);
}

bool ImpParser::isConst(Term* term)
{
  if (!term)
    return true;
  if (term->type == VARIABLE || term->type == RANDOM)
    return false;
  return isConst(term->arg[0]) && isConst(term->arg[1]);
}

void ImpParser::getVars(Term* term, OdStringSet& usedVars)
{
  if (!term)
    return;
  getVars(term->arg[0], usedVars);
  getVars(term->arg[1], usedVars);
  if (term->type == VARIABLE)
    usedVars.insert(term->name);
}

bool ImpParser::isConstExpression()
{
  return isConst(m_root);
}

OdResult ImpParser::getVariables(OdStringArray& usedVars)
{
  bool emptyUsed = usedVars.isEmpty();
  OdStringSet vars;
  getVars(m_root, vars);
  vars.erase(s_constPi);
  vars.erase(s_constE);
  OdStringSet::iterator it;
  for (it = vars.begin(); it != vars.end(); ++it) {
    if (emptyUsed || !usedVars.contains(*it))
      usedVars.append(*it);
  }
  return eOk;
}

bool ImpParser::store(const OdChar* &pChar, CharCheck f)
{
  if (!pChar && f(m_pChar[-1]))
    pChar = m_pChar - 1;
  return true;
}

void ImpParser::revert(const OdChar* pWord)
{
  int n = int(m_pChar - pWord);
  m_translated.deleteChars(m_translated.getLength() - n, n);
  m_pChar = pWord;
}

static OdResult getDouble(OdDbEvalVariantPtr variant, double& value)
{
  if (variant.isNull())
    return eNullPtr;
  OdResult res = variant->getValue(value);
  if (res != eOk) {
    int intVal;
    res = variant->getValue(intVal);
    if (res != eOk)
      return res;
    value = intVal;
  }
  return eOk;
}

Parser::Parser() : m_pImpl(new ImpParser()) {}

Parser::~Parser() { delete m_pImpl; m_pImpl = 0; }

OdResult Parser::setName(const OdString& name)
{
  return m_pImpl->setName(name);
}

OdResult Parser::addVariable(const OdString& name, OdDbEvalVariantPtr value)
{
  double val;
  return (getDouble(value, val) == eOk) ? m_pImpl->addVariable(name, val) : eInvalidInput;
}

OdResult Parser::fixVariable(const OdString& name, OdDbEvalVariantPtr value)
{
  double val;
  return (getDouble(value, val) == eOk) ? m_pImpl->fixVariable(name, val) : eInvalidInput;
}

OdResult Parser::renameVariable(const OdString& oldName, const OdString& newName)
{
  return m_pImpl->renameVariable(oldName, newName);
}

OdResult Parser::setExpression(const OdString& expression)
{
  return m_pImpl->setExpression(expression);
}

OdDbEvalVariantPtr Parser::evaluate()
{
  try {
    double val = m_pImpl->evaluate();
    return ((int)val == val) ? OdDbEvalVariant::init((int)val) : OdDbEvalVariant::init(val);
  }
  catch (...) {
    return OdDbEvalVariantPtr();
  }
}

bool Parser::isConstExpression()
{
  return m_pImpl->isConstExpression();
}

OdResult Parser::getVariables(OdStringArray& usedVars)
{
  return m_pImpl->getVariables(usedVars);
}

OdString Parser::getTranslated()
{
  return m_pImpl->getTranslated();
}

OdString createExpression(OdDbEvalVariantPtr value)
{
  OdString res;
  double d;

  if (getDouble(value, d) == eOk)
    res.format(OD_T("%g"), d);

  return res;
}
} // namespace OdExpression
