shader_type canvas_item;

uniform vec2 speed = vec2(0.2, 0.0);
uniform vec2 tiling = vec2(3.0, 3.0);

uniform float alpha: hint_range(0.0, 1.0) = 1.0;

uniform vec4 mixColor: source_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform float mixAmount: hint_range(0.0, 1.0);

// -- Fog --
uniform vec4 fogCol: source_color = vec4(1.0, 1.0, 1.0, 1.0);
uniform float fogIntensity = 0.0;
uniform float fogOffset = 0.0;
uniform float fogStart = 0.0;
uniform float fogEnd = 1.0;

void fragment()
{
	vec4 color = texture(TEXTURE, mod(UV * tiling + TIME * speed, 1.0));
	color = mix(color, mixColor, mixAmount);
	color.a *= alpha;

	// Add "fog"
	float fogCoord = clamp(fogOffset + (fogStart + UV.y * fogEnd), 0.0, 1.0);
	color = mix(color, fogCol, fogCoord * fogIntensity);

	COLOR = color;
}