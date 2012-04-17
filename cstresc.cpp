// cstresc.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>

const char help_message[] = 
  "cstresc - C/C++ string esaper/unescaper\n"
  "using: cstresc [-e|-u][-w] [input [output]]\n"
  "  -e --escape - escape normal string for using in C/C++ surces (default)\n"
  "  -u --unescape - unescape from C/C++ surces\n"
  "  input - input file name, defaul - stdin\n"
  "  output - output file name, defaul - stdout\n"
  "freeware\n";

void show_help(void)
{
  std::cout << help_message;
}

void escape_string(std::istream & IN, std::ostream & OUT)
{
  char work_char;
  OUT << '\"';
  while (IN.get(work_char))
  {
    switch (work_char)
    {
    case '\n': OUT << "\\n\"\n\""; break;
    case '\t': OUT << "\\t"; break;
    case '\v': OUT << "\\v"; break;
    case '\b': OUT << "\\b"; break;
    case '\r': OUT << "\\r"; break;
    case '\f': OUT << "\\f"; break;
    case '\a': OUT << "\\a"; break;
    case '\\': OUT << "\\\\"; break;
    //case '\?': OUT << "\\?"; break;
    case '\'': OUT << "\'"; break;
    case '\"': OUT << "\\\""; break;
    case '\0': OUT << "\\0"; break;
    default:
      if (work_char >= 0 && work_char < 32)
      {
        char buf[32];
        sprintf(buf, "\\x%02x", work_char);
        OUT << buf;
      }
      else
        OUT << work_char;
      break;
    }
  }
  OUT << '\"';
}

void unescape_string(std::istream & IN, std::ostream & OUT)
{
  /*
  TODO: add numeric codes
  TODO: comments support
  TODO: line brakes with "\"
  TODO: margin 80
  */
  enum {st_out, st_in, st_escape/*,st_comment, st_octal, st_hex*/} state = st_out;
  //int char_code;
  char work_char;
  bool double_quote;
  while (IN.get(work_char))
  {
    //begin_parse:
    switch (state)
    {
    case st_out:
      switch (work_char)
      {
        case '\"': state = st_in; double_quote = true; break;
        case '\'': state = st_in; double_quote = false; break;
      };
      break;
    case st_in:
      switch (work_char)
      {
      case '\\': state = st_escape; break;
      case '\"':
        if (double_quote)
          state = st_out;
        else
          OUT << work_char;
      break;
      case '\'':
        if (!double_quote)
          state = st_out;
        else
          OUT << work_char;
      break;
      default: OUT << work_char; break;
      }
      break;
    case st_escape:
      switch (work_char)
      {
      case 'n': OUT << "\n"; break;
      case 't': OUT << "\t"; break;
      case 'v': OUT << "\v"; break;
      case 'b': OUT << "\b"; break;
      case 'r': OUT << "\r"; break;
      case 'f': OUT << "\f"; break;
      case 'a': OUT << "\a"; break;
      case '0': OUT << "\0"; break;
      //case '1': char_code = 1; state = st_octal; break;
      //case '2': char_code = 2; state = st_octal; break;
      //case '3': char_code = 3; state = st_octal; break;
      //case '4': char_code = 4; state = st_octal; break;
      //case '5': char_code = 5; state = st_octal; break;
      //case '6': char_code = 6; state = st_octal; break;
      //case '7': char_code = 7; state = st_octal; break;
      //case '8': char_code = 8; state = st_octal; break;
      //case '9': char_code = 9; state = st_octal; break;
      //case 'x': state = st_hex; break;
      default: OUT << work_char; break;
      }
      if (state == st_escape) state = st_in;
      break;
    //case st_octal:
    //  break;
    //case st_hex:
    //  break;
    }
  }
}

int _tmain(int argc, _TCHAR* argv[])
{
  bool unescape = false;
  std::istream * in = 0;
  std::ostream * out = 0;
  for(int i = 1; i < argc; ++i)
  {
    if (argv[i][0] == '-')
    {
      if (!unescape && (strcmp(argv[i],"-u") == 0 || strcmp(argv[i],"--unescape") == 0))
        unescape = true;
    }
    else
    {
      if (!in)
        in = new std::ifstream(argv[i]);
      else if (!out)
        out = new std::ofstream(argv[i]);
    }
  }

  if (unescape)
    unescape_string((in ? *in : std::cin), (out ? *out : std::cout));
  else
    escape_string((in ? *in : std::cin), (out ? *out : std::cout));

  if (in) delete in;
  if (out) delete out;

  return 0;
}

