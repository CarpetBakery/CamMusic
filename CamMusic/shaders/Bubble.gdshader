shader_type canvas_item;
render_mode blend_add;

uniform float alpha = 1.0;
uniform float alphaOffset = 1.0;

void fragment()
{
	vec4 col = COLOR;
	col.a *= sin(TIME * 3.0 + UV.y) * 0.25 + 1.3;
	col.a *= alpha * alphaOffset;

	COLOR = col;
}