
class Leaf_4b5500 { public: unsigned int get(unsigned int index); };
unsigned int Leaf_4b5500::get(unsigned int index) { unsigned int vector_or_index = (unsigned int)this; unsigned int buffer_or_element; unsigned int unused_value; buffer_or_element = *(unsigned int*)(vector_or_index + 4); vector_or_index = index; buffer_or_element = *(unsigned int*)(buffer_or_element + vector_or_index*4); return buffer_or_element; }
