#! /usr/bin/env python3
# @author Dominik Dziuba
# History of changes
# Version - Author - Change
# v1        Dominik   Initial version
import argparse
import json
from ModuleRunner import ModuleRunner
import jsonschema
from pathlib import Path

# TODO: fix this schema for current project logic
schema_string = """
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "title": "moduleDefinitionsSchema",
  "type": "object",
  "patternProperties": {
  	"[A-Za-z]+": {
      "type": "object",
      "properties": {
      	"methodToCall": {"type": "string"},
        "info": {
          "type": "object",
          "properties": {
            "path": {"type": "string"},
            "options": {
              "type": "array",
              "items": {
                "type": "object",
                "patternProperties": {
                  "^(option|help|required|action|nargs|const|default|type|choices|metavar|dest)": {
                    "type": "string"
                  }
                },
                "required": ["option"]
              }
            },
            "help": {"type": "string"}
          },
          "required": ["path", "options", "help"]
        }
      },
      "required": ["methodToCall", "info"]
    }
  }
}
"""


def main():
    # options for overall program
    parser = argparse.ArgumentParser(description='This is a program for classifying images and verifying different methods of doing so.')

    parser.add_argument('-p', '--path', help='This defines the path to the images.', default='.', type=str)
    parser.add_argument('-r', '--recursive',
                        help='This defines if image search is limited to directory passed in the path variable.',
                        default=False, action='store_true')
    parser.add_argument('-v', '--validation', help='This variable can contain path to supported file to validate module output.',
                        default=None, type=str)
    parser.add_argument('-z', '--validator', help="This cariable can contain path to validating module.",
                        default=None, type=str)

    # loading module definitions
    moduleDefinitions = load_module_definitions()

    # validating loaded module definitions to fit schema
    # jsonschema.validate(moduleDefinitions, json.loads(schema_string))

    subparsers = parser.add_subparsers(dest='moduleName', required=True)
    for definition in moduleDefinitions:
        sub = subparsers.add_parser(definition['name'], help=definition['info']['help'])
        for arg in definition['info']['options']:
            optionName = arg['option']
            del arg['option']
            sub.add_argument(optionName, **arg)

    options = parser.parse_args()
    optionsDict = vars(options)

    if not optionsDict:
        pass
        # no options, load preconfigured json file with options
    else:
        for definition in moduleDefinitions:
            if definition['name'] == optionsDict['moduleName']:
                ModuleRunner.run(optionsDict, definition)
                break


def load_global_config():
    pass


def load_module_definitions():
    result = []
    moduleDefinitionPathGen = Path('./Modules').glob('*/moduleDefinition.json')
    for p in moduleDefinitionPathGen:
        with open(p, 'r') as file:
            definition = json.load(file)
        result.append(definition)
    return result


if __name__ == "__main__":
    main()