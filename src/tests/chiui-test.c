#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <xml.h>

int main() {
  char buf1[512], buf2[512];
  sscanf("style=\"color: red\" ", "%[^=]=\"%[^\"]", buf1, buf2);
  printf("%s=%s\n", buf1, buf2);
  uint8_t *source = (uint8_t *) "\
<html>\
  <body>\
    <h1>Test Title</h1>\
    <p fullWidth=\"Yes\" style=\"color: red; width: 100%; height: 200%\">Test Paragraphy</p>\
    <p>Test Paragraphy</p>\
  </body>\
</html>\
  ";
  xml_dom *dom = xml_parse_document(source, strlen((char *) source));
  if (!dom) exit(EXIT_FAILURE);
  xml_node *root = xml_document_root(dom);
  xml_node *body = xml_node_child(root, 0);
  xml_node *child = NULL;
  size_t child_cnt = 0;
  while ((child = xml_node_child(body, child_cnt++)) != NULL) {
    xml_str *name = xml_node_name(child), *text = xml_node_content(child);
    uint8_t *name_buffer = calloc(xml_string_length(name) + 1, sizeof(uint8_t));
    uint8_t *text_buffer = calloc(xml_string_length(text) + 1, sizeof(uint8_t));
    size_t attr_count = xml_node_attributes(child);
    xml_string_copy(name, name_buffer, xml_string_length(name));
    xml_string_copy(text, text_buffer, xml_string_length(text));
    printf("<%s>%s</%s>\n", name_buffer, text_buffer, name_buffer);
    for (size_t i = 0; i < attr_count; i++) {
      xml_str *attr_name = xml_node_attribute_name(child, i), *attr_content = xml_node_attribute_content(child, i);
      uint8_t *attr_name_buffer = calloc(xml_string_length(attr_name) + 1, sizeof(uint8_t));
      uint8_t *attr_content_buffer = calloc(xml_string_length(attr_content) + 1,
                                            sizeof(uint8_t));
      xml_string_copy(attr_name, attr_name_buffer, xml_string_length(attr_name));
      xml_string_copy(attr_content, attr_content_buffer, xml_string_length(attr_content));
      printf("\t%s = %s\n", attr_name_buffer, attr_content_buffer);
      free(attr_name_buffer);
      free(attr_content_buffer);
    }
    free(name_buffer);
    free(text_buffer);
  }
  xml_document_free(dom, false);
  return 0;
}