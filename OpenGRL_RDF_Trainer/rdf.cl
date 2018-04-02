__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

struct __attribute__ ((packed)) Decision {
    int2 u;
    int2 v;
    float t;
};

struct __attribute__ ((packed)) Pixel {
    short2 coords;
    float depth;
    uint img_id;
    char index;
};

__kernel void getFeatureTrain(
    __read_only image2d_array_t images,
    __global const struct Pixel *pixels,
    __read_only struct Decision decision,
    __global char *split_buffer,
    __read_only int id_offset)
{
    const int id = get_global_id(0);
    const struct Pixel pixel = pixels[id];
    const int img_id = pixel.img_id - id_offset;
    const int4 coords_p = (int4)(pixel.coords.x, pixel.coords.y, img_id, 0);
    const float depth_p = read_imagef(images, sampler, coords_p).x;


    const int4 coords_u = (int4)(pixel.coords.x + round(decision.u.x/depth_p),
                                 pixel.coords.y + round(decision.u.y/depth_p),
                                 img_id, 0);
    const float4 depth_u = read_imagef(images, sampler, coords_u);
    const int4 coords_v = (int4)(pixel.coords.x + round(decision.v.x/depth_p),
                                 pixel.coords.y + round(decision.v.y/depth_p),
                                 img_id, 0);
    const float4 depth_v = read_imagef(images, sampler, coords_v);

    if (depth_u.x >= 100.0f || depth_v.x >= 100.0f || depth_u.x == 0.0f || depth_v.x == 0.0f) {
        // Go right if out of range
        split_buffer[id] = 1;
    } else {
        split_buffer[id] = depth_u.x - depth_v.x < decision.t ? -1 : 1;
    }
}

__kernel void getProbabilities(
    __global const struct Pixel *pixels,
    __global uint *probabilities)
{
    const int id = get_global_id(0);
    const char index = pixels[id].index;
    atomic_inc(&probabilities[index]);
}
