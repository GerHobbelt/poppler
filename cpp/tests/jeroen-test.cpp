/* Example of encoding problem with ustrings.
 *
 * Compile using:
 *   g++ -std=c++11 test.cpp $(pkg-config --cflags --libs poppler-cpp)
 *
 */
#include <poppler-document.h>
#include <poppler-page.h>
#include <poppler-toc.h>

#include <iostream>

using namespace poppler;

void print_ustring(ustring x){
  if(x.length()){
    byte_array buf = x.to_utf8();
    std::string out(buf.data(), buf.size());
    std::cout << out.c_str() <<  std::endl;
  }
}

int main(int argc, char* argv[]){
  if (argc < 2)
    throw std::runtime_error("Use pdf filename as parameter");
  document * doc = document::load_from_file(argv[1], "", "");
  if(!doc)
    throw std::runtime_error("Failed to read pdf file");
  page *p(doc->create_page(0));
  if(!p)
    throw std::runtime_error("Failed to create pagee");

  //Properly gets converted to UTF8:
  std::cout << "Actual text:\n" <<  std::endl;
  print_ustring(p->text());
  std::cout << "\n--------\n Some ustring fields:\n" <<  std::endl;

  //Incorrect conversion to UTF8:
  print_ustring(doc->get_creator());
  print_ustring(doc->get_author());
  print_ustring(doc->create_toc()->root()->title());
  std::cout << "\n--------\n Individual textboxes:\n" <<  std::endl;

  //Also incorrect conversion to UTF8:
  std::vector<text_box> words = p->text_list();
  for(int i = 0; i < words.size(); i++)
    print_ustring(words.at(i).text());
  return 0;
}
