import sys
from typing import TextIO


def define_ast(output_dir: str, base_name: str, types: list[str]) -> None:
    path = output_dir + "/" + base_name.lower() + ".h"

    with open(path, "w") as f:
        f.write("#pragma once\n")
        f.write('#include "token.h"\n')
        f.write("namespace fex {\n")

        for type in types:
            class_name = type.split(":")[0].strip()
            f.write(f"class {class_name};\n")

        f.write("\n")

        define_visitor(f, base_name, types)

        # base class
        f.write(
            f"class {base_name} {{\n"
            "  public:\n"
            f"    virtual ~{base_name}() {{}}\n"
            "    virtual void Accept(Visitor& visitor) const = 0;\n"
            "};\n"
        )

        for type in types:
            class_name = type.split(":")[0].strip()
            fields = type.split(":")[1].strip()
            define_type(f, base_name, class_name, fields)

        f.write("} // namespace fex\n")


def define_visitor(f: TextIO, base_name: str, types: list[str]) -> None:
    f.write("class Visitor {\n")
    f.write("  public:\n")
    f.write("    virtual ~Visitor() {}\n")

    for type in types:
        type_name = type.split(":")[0].strip()
        f.write(
            f"    virtual void Visit{type_name}{base_name}(const {type_name}* {base_name.lower()}) = 0;\n"
        )

    f.write("};\n")


def define_type(f: TextIO, base_name: str, class_name: str, field_list: str) -> None:
    f.write(f"class {class_name} : public {base_name} {{\n")
    f.write("  public:\n")

    # constructor
    f.write(f"    {class_name}({field_list})\n")
    f.write("        :")
    fields = [f.strip() for f in field_list.split(",")]
    for i, field in enumerate(fields):
        type_part, name = field.rsplit(" ", 1)
        f.write(f" m_{name}({name})")
        if i != len(fields) - 1:
            f.write(",")
    f.write(" {}\n")

    # visitor pattern
    f.write(
        f"    virtual void Accept(Visitor& visitor) const override {{ visitor.Visit{class_name}{base_name}(this); }}\n"
    )

    # members
    f.write("  private:\n")
    for field in fields:
        type_, name = field.rsplit(" ", 1)
        f.write(f"    {type_} m_{name};\n")

    f.write("};\n")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 generate_ast <output directory>")
        exit()

    output_dir = sys.argv[1]

    exprs = [
        "Assign   : Token name, const Expr* value",
        "Binary   : const Expr* left, Token op, const Expr* right",
        "Grouping : const Expr* expression",
        "Literal  : Value value",
        "Logical  : const Expr* left, Token op, const Expr* right",
        "Unary    : Token op, const Expr* right",
        "Variable : Token name",
    ]

    define_ast(output_dir, "Expr", exprs)
