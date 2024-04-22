#ifndef GLOBAL_H
#define GLOBAL_H

#include <algorithm>
#include <vector>

#include "EvaValue.h"

struct GlobalVar {
  std::string name;
  EvaValue value;
};

struct Global {
  const GlobalVar& get(size_t index) const { return globals[index]; }

  void set(size_t index, const EvaValue& value) {
    if (index >= globals.size()) {
      throw std::runtime_error("Global does not exist");
    }
    globals[index].value = value;
  }

  void define(const std::string& name) {
    if (exists(name)) {
      return;
    }
    globals.push_back({name, EvaValue(0.0)});
  }

  void addConst(const std::string& name, const EvaValue& value) {
    if (exists(name)) {
      return;
    }
    globals.push_back({name, value});
  }

  int getGlobalIndex(const std::string& name) const {
    const auto it = std::find_if(
        globals.begin(), globals.end(),
        [&name](const GlobalVar& var) { return var.name == name; });
    if (it != globals.end()) {
      return std::distance(globals.begin(), it);
    }
    return -1;
  }

  bool exists(const std::string& name) const {
    return getGlobalIndex(name) != -1;
  }

  std::vector<GlobalVar> globals;
};

#endif
