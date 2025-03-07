#version 430
layout(local_size_x = 32, local_size_y = 32) in;

layout(binding = 0, rgba32f) uniform readonly image2D worldPosition;

layout(std430, binding = 0) coherent buffer ssbo {
	uvec2 cell[];
};

uniform ivec2 resolution;
uniform ivec2 grid_size;
uniform vec3 eye_position;

void main()
{
	// ivec2 grid_size = ivec2(5);
	ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

	uint gridSizeX = uint(ceil(float(resolution.x) / grid_size.x));
	uint gridSizeY = uint(ceil(float(resolution.y) / grid_size.y));
	uint cellID =  (gl_GlobalInvocationID.y / gridSizeY) * grid_size.y + (gl_GlobalInvocationID.x / gridSizeX);

	vec3 worldPos = imageLoad(worldPosition, pixel).xyz;
	if (worldPos.x == 0 && worldPos.y == 0 && worldPos.z == 0)
		return;
	
	uint dist = uint(length(worldPos - eye_position) * 1000);
	
	if (dist > 0) {
		atomicMin(cell[cellID].x, dist);
		atomicMax(cell[cellID].y, dist);
	}
}
