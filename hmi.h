struct _Product {
    char[50];
    unsigned int harga_agen;
    unsigned int harga_konsumen;
};

void append_log(const char *title, const char *state);

void add_product(const char *database);
void remove_product(const char *database);
