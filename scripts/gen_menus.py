#!/usr/bin/python3

import json
import os
from argparse import ArgumentParser


def main():
    argument_parser = ArgumentParser()
    argument_parser.add_argument("input_file")
    argument_parser.add_argument("out_h_file")
    argument_parser.add_argument("out_c_file")
    args = argument_parser.parse_args()

    with (
        open(args.input_file) as ifp,
        open(args.out_h_file, "w") as outh,
        open(args.out_c_file, "w") as outc,
    ):
        data = json.load(ifp)

        menu_var_name_type: dict[str, str] = {}
        menu_body_enumerators = set()
        menu_body_texts = {}
        menu_body_options: dict[str, list[str | tuple[str, str]]] = {}
        menu_option_enumerators = set()
        menu_option_texts = {}
        menu_option_next_menu = {}
        menu_option_custom_code = {}

        for row in data:
            if row[0] == "var":
                if len(row) != 3:
                    raise ValueError(row)
                name = row[1]
                type_ = row[2]
                if name in menu_var_name_type:
                    raise ValueError(f"redeclaration of variable {name}")
                menu_var_name_type[name] = type_

            elif row[0] == "menu":
                if len(row) != 4:
                    raise ValueError(row)
                _, enumerator, text, options = row
                menu_body_enumerators.add(enumerator)
                menu_body_texts[enumerator] = text
                menu_body_options[enumerator] = options
                for o in [o for o in options if isinstance(o, list)]:
                    if len(o) != 2 or not (
                        isinstance(o[0], str) and isinstance(o[1], str)
                    ):
                        raise ValueError(o)

            elif row[0] == "option":
                if len(row) != 5:
                    raise ValueError(row)
                _, enumerator, text, next_menu, custom_code = row
                menu_option_enumerators.add(enumerator)
                menu_option_texts[enumerator] = text
                menu_option_next_menu[enumerator] = next_menu
                menu_option_custom_code[enumerator] = custom_code

            else:
                raise ValueError(row)

        sorted_menu_enumerators = sorted(menu_body_enumerators)
        sorted_option_enumerators = sorted(menu_option_enumerators)

        outh.write("/* This file is auto-generated. Do not edit manually! */\n")
        outh.write("#ifndef ELYSIUM_MENUS_H\n")
        outh.write("#define ELYSIUM_MENUS_H\n\n")

        outh.write('#include "constants.h"\n')
        outh.write('#include "types.h"\n\n')

        outh.write("struct menu_vars {\n")
        for name, type_ in menu_var_name_type.items():
            outh.write(f"    {type_} {name};\n")
        outh.write("};\n\n")

        outh.write("extern struct menu_vars menu_vars;\n\n")

        outh.write("enum menu_body_id {\n")
        for i, enumerator in enumerate(sorted_menu_enumerators):
            outh.write(f"    {enumerator},\n")
        outh.write("    N_MENU_BODIES\n")
        outh.write("};\n\n")

        outh.write("enum menu_option_id {\n")
        for i, enumerator in enumerate(sorted_option_enumerators):
            outh.write(f"    {enumerator},\n")
        outh.write("    N_MENU_OPTIONS\n")
        outh.write("};\n\n")

        outh.write(
            "extern char menu_body_texts[N_MENU_BODIES][MAX_MENU_BODY_TEXT_LEN];\n\n"
        )
        outh.write(
            "extern char menu_option_texts[N_MENU_OPTIONS][MAX_MENU_OPTION_TEXT_LEN];\n\n"
        )

        outh.write("extern void open_menu(enum menu_body_id menu_body_id);\n\n")
        outh.write("extern void select_option(enum menu_option_id option_id);\n\n")

        outh.write("#endif\n")

        outc.write("/* This file is auto-generated. Do not edit manually! */\n")
        outc.write(f'#include "{os.path.basename(args.out_h_file)}"\n')
        outc.write(f'#include "game.h"\n')
        outc.write(f'#include "menu_manual.h"\n')
        outc.write(f'#include "ncurses.h"\n\n')

        outc.write("struct menu_vars menu_vars;\n\n")

        outc.write("char menu_body_texts[N_MENU_BODIES][MAX_MENU_BODY_TEXT_LEN] = {\n")
        for i, enumerator in enumerate(sorted_menu_enumerators, 1):
            outc.write(f'        "{menu_body_texts[enumerator]}"')
            if i != len(menu_body_enumerators):
                outc.write(",")
            outc.write("\n")
        outc.write("};\n\n")

        outc.write(
            "char menu_option_texts[N_MENU_OPTIONS][MAX_MENU_OPTION_TEXT_LEN] = {\n"
        )
        for i, enumerator in enumerate(sorted_option_enumerators, 1):
            outc.write(f'        "{menu_option_texts[enumerator]}"')
            if i != len(menu_option_enumerators):
                outc.write(",")
            outc.write("\n")
        outc.write("};\n\n")

        outc.write("void open_menu(enum menu_body_id menu_body_id) {\n")
        outc.write("        int i = 0;\n")
        outc.write("        game.menu_body_id = menu_body_id;\n")
        outc.write("        game.i_focused_option = 0;\n")
        outc.write("        switch (menu_body_id) {\n")
        for enumerator in sorted_menu_enumerators:
            outc.write(f"        case {enumerator}:\n")
            for option in menu_body_options[enumerator]:
                if isinstance(option, str):
                    outc.write(
                        f"                game.available_menu_option_ids[i++] = {option};\n"
                    )
                elif isinstance(option, list):
                    outc.write(f"                if ({option[0]}) {{\n")
                    outc.write(
                        f"                    game.available_menu_option_ids[i++] = {option[1]};\n"
                    )
                    outc.write("                }\n")
            outc.write("                break;\n")
        outc.write("        case N_MENU_BODIES: break;\n")
        outc.write("        }\n")
        outc.write("        game.n_available_menu_option_ids = i;\n")
        outc.write("}\n\n")

        outc.write("void select_option(enum menu_option_id option_id) {\n")
        outc.write("        switch (option_id) {\n")
        for enumerator in sorted_option_enumerators:
            outc.write(f"        case {enumerator}:\n")
            if custom_code := menu_option_custom_code[enumerator]:
                outc.write(f"                {custom_code}\n")
            if next_menu_id := menu_option_next_menu[enumerator]:
                outc.write(f"                open_menu({next_menu_id});\n")
            outc.write("                break;\n")
        outc.write("        case N_MENU_OPTIONS: break;\n")
        outc.write("        }\n")
        outc.write("}\n")


if __name__ == "__main__":
    main()
