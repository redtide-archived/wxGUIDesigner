LANGUAGE_NAME="PHP"
LANGUAGE_ICON="php.png"
FILE_EXTENSION="php"
GENERATOR_AUTHOR="Jefferson González"

function SourceHeader (file)
    return "/* " .. file .. " generated with wxGD */\n"
end

function ClassStart(class, parent)
	code = "/**" .. "\n" ..
	" * Class " .. class .. "\n" ..
	" */" .. "\n" ..
	"class " .. class .. " extends " .. parent .. "\n" ..
	"{" .. "\n"
	
	return code
end

function ClassEnd(class, parent)
	return "}\n"
end
	
