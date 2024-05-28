import json
from typing import Any

import requests

# url for the site to make api call
api_site: str = "https://api.vns.wiki"

# url for getting vns schema
schema_url: str = f"{api_site}/get/schema"

# send request
schema_response: requests.Response = requests.get(schema_url)

# show result
print("VNS online service responded schema request with:")
print(json.loads((schema_response.text)))

# an example of vns script in string
raw_vns_content: str = """
# Fundamental parameters
[id]1
[language]English

[section]dialog_example

// section 1
[bgi]bg1.png
[bgm]bgm1.ogg
# this the first dialogue of section 1
null:
- This is a test message.

section1_end = true
chapter_passed = 1
chapter_passed++
chapter_passed*=3
chapter_passed/=3
chapter_passed--

# this the second dialogue of section 1
null:
- I hope it works.

mod_n = chapter_passed + 1
test_multi = 1 + mod_n * 4
mod_result = test_multi % 2

[block]

null:
- you cannot go back

// section 2
[scene]bg2.png
[bgm]bgm2.ogg
[display]test_character1.png test_character2.png
# this the first dialogue of section 2
Test Character 1:
- Hello I am test character 1.
# this the second dialogue of section 2
Test Character 2:
- Hello I am test character 2.
[option]click here to go to jump point 1 -> jumping_point1
[option]click here to go to the end -> last_one

[label]last_one
Test Character 2:
- Ok this is the end

[end]

[label]jumping_point1
say = "hi"

Test Character 3:
- Welcome to jump point 1

Test Character 3:
- My previous should be jump point 1
- And the next should be None
"""

# url for compiling raw vns script
compiler_url: str = f"{api_site}/post/compile"

# send request
compiler_response: requests.Response = requests.post(
    compiler_url, json={"data": raw_vns_content}
)

# the json data get from compiling the raw vns script
json_compiled: dict[str, Any] = json.loads(compiler_response.text)

# show result
print("\nVNS online service responded compiler request with:")
print(json_compiled)

# url for validating compiled vns json file
validation_url: str = f"{api_site}/post/validate"

# send request
validation_response: requests.Response = requests.post(
    validation_url, json=json_compiled
)

# obtain validation result
validation_result: dict[str, bool] = json.loads(validation_response.text)

# assert the result is True
assert validation_result.get("result", False) is True
