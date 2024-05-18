#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <nlohmann/json-schema.hpp>
#include <iostream>

const nlohmann::json VNS_SCHEMA = R"(
    {
        "$schema": "http://json-schema.org/draft-07/schema#",
        "properties": {
            "compiler": {
                "properties": {
                    "compiledAt": {
                        "type": "integer"
                    },
                    "reversion": {
                        "type": "integer"
                    },
                    "version": {
                        "type": "integer"
                    }
                },
                "required": [
                    "compiledAt",
                    "reversion",
                    "version"
                ],
                "type": "object"
            },
            "dialogs": {
                "additionalProperties": {
                    "properties": {
                        "background_image": {
                            "type": "string"
                        },
                        "background_music": {
                            "type": "string"
                        },
                        "character_images": {
                            "items": {
                                "type": "string"
                            },
                            "type": "array"
                        },
                        "comments": {
                            "items": {
                                "type": "string"
                            },
                            "type": "array"
                        },
                        "contents": {
                            "items": {
                                "type": "string"
                            },
                            "type": "array"
                        },
                        "narrator": {
                            "type": "string"
                        },
                        "next": {
                            "properties": {
                                "target": {
                                    "oneOf": [
                                        {
                                            "type": "string"
                                        },
                                        {
                                            "items": {
                                                "properties": {
                                                    "id": {
                                                        "type": "string"
                                                    },
                                                    "text": {
                                                        "type": "string"
                                                    }
                                                },
                                                "required": [
                                                    "id",
                                                    "text"
                                                ],
                                                "type": "object"
                                            },
                                            "type": "array"
                                        }
                                    ]
                                },
                                "type": {
                                    "type": "string"
                                }
                            },
                            "required": [
                                "type"
                            ],
                            "type": "object"
                        },
                        "previous": {
                            "type": "string"
                        }
                    },
                    "type": "object"
                },
                "type": "object"
            },
            "id": {
                "type": "string"
            },
            "language": {
                "type": "string"
            }
        },
        "required": [
            "compiler",
            "dialogs",
            "id",
            "language"
        ],
        "type": "object"
    }
)"_json;

class Validator
{
    const nlohmann::json_schema::json_validator VALIDATOR;

public:
    Validator() : VALIDATOR(VNS_SCHEMA)
    {
    }

    // validate whether given json object is valid vsn format dialogue
    [[nodiscard]] bool validate(const nlohmann::json &) const;

    // validate whether a json file has valid vsn format dialogue
    [[nodiscard]] bool validate(const std::filesystem::path &) const;
};

#endif
