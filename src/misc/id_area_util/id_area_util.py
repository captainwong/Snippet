
import csv
from collections import namedtuple


    
with open("area.h", "wt", encoding="utf-8-sig") as area_h:
    area_h.write("""#pragma once

#include <unordered_map>
#include <string>

static const std::unordered_map<std::string, std::string> Area = {
""")

    with open("area.csv", encoding="utf-8") as f:
        ff = csv.reader(f)
        headings = next(ff)
        Row = namedtuple('Row', headings)
        for r in ff:
            row = Row(*r)
            print("{}, {}".format(row.area_id, row.area_name))
            area_h.write('  {{ "{}", "{}" }}, \n'.format(row.area_id, row.area_name))

    area_h.write("};\n")