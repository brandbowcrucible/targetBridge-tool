#ifndef TB_I18N_H
#define TB_I18N_H

#include <stddef.h>

struct tb_i18n_pair {
    const char *name;
    const char *value;
};

int tb_i18n_init(void);
const char *tb_i18n_get(const char *key);
void tb_i18n_set_runtime_language(const char *language_code);
const char *tb_i18n_current_language(void);
void tb_i18n_format(char *dest,
                    size_t size,
                    const char *key,
                    const struct tb_i18n_pair *pairs,
                    size_t pair_count);

#endif
