#ifndef VALUE_H
#define VALUE_H

template <class T>
T value(const QVariant &var, const T &defaultValue, bool *pOk = 0) {
    bool ok = var.isValid() && var.canConvert<T>();
    if (pOk) {
        *pOk = ok;
    }
    if (ok) {
        return var.value<T>();
    } else {
        return defaultValue;
    }
}

#endif // VALUE_H
