#version 330 core

in vec2 z;
out vec4 col;

uniform vec2 mouse;
uniform vec2 lastMouseClick;
uniform vec2 centre;
uniform float zoom;
uniform float clicked;


float Pi = 3.14159265358979f;
vec2 I = vec2(0.0f, 1.0f);
vec2 one = vec2(1.0f, 0.0f);

float weight(float x, float centre, float deviation){
    float result = (x-centre)*(x-centre)/(deviation*deviation) + 1.0f;
    return 1.0f/result;
}

vec3 myFreq_to_RGB(float brightness, float freq){
    
    float t = 8.0f;
    int n = 2;
    
    float fr1 = t*freq;
    float fr2 = t*(freq - 1);
    float fg  = t*(freq - 0.25f);

    vec3 frqV = vec3(fr1, fr2, fg);
    
    frqV *= frqV;
    frqV = vec3(pow(frqV.x, n), pow(frqV.y, n), pow(frqV.z, n));

    float r = 1.0f / (1 + frqV.x) + 1.0f / (1 + frqV.y);
    float g = 1.0f / (1 + frqV.z);
    float b = 1.0f - r - g;

    return brightness * vec3(r,g,b);

}

vec3 HSL_to_RGB(vec3 colour){
    float h = colour.x;
    float s = colour.y;
    float l = colour.z;

    vec3 rgb;

    float angle = Pi/3.0f;

    if(h <= angle)
        rgb = vec3(1, 1, 0)*h + vec3(1, 0, 0)*(angle - h);// Red to Yellow, 0 to 60

    if(angle < h && h <= 2 * angle)
        rgb = vec3(0, 1, 0)*(h - angle) + vec3(1, 1, 0) * (2 * angle - h);// Yellow to Green, 60 to 120

    if(2 * angle < h && h <= 3 * angle)
        rgb = vec3(0, 1, 1)*(h - 2 * angle) + vec3(0, 1, 0)*(3 * angle - h);// Green to Turquoise, 120 to 180

    if(3 * angle < h && h <= 4 * angle)
        rgb = vec3(0, 0, 1)*(h - 3 * angle) + vec3(0, 1, 1)*(4 * angle - h);// Turquoise to Blue, 180 to 240

    if(4 * angle < h && h <= 5 * angle)
        rgb = vec3(1, 0, 1)*(h - 4 * angle) + vec3(0, 0, 1)*(5 * angle - h);// Blue to Purple, 240 to 300
    
    if(5 * angle < h && h <= 6 * angle)
        rgb = vec3(1, 0, 0)*(h - 5 * angle) + vec3(1, 0, 1)*(6 * angle - h);// Purple to Red, 300 to 360

    rgb = rgb / angle;

    rgb = rgb*s + (1.0f - s)*vec3(1.0f/3.0f);

    if(l > 0.5f)
        rgb = vec3(1)*(l - 0.5f) + (1.0f - l)*rgb;
    else
        rgb = rgb*l + (0.5f - l)*vec3(0);
    rgb = rgb / 0.5f;

    return rgb;
}
vec3 RGB_to_HSL(vec3 colour){
    return vec3(0,0,0);
}

vec3 getColor(vec2 point){

    float angle = atan(point.y, point.x) + Pi;
    float rSq = point.x * point.x + point.y * point.y;
    //1.0f - 1.0f / (1.0f + rSq)
    //2/Pi * atan(sqrt(rSq))

    vec3 hsl = vec3(mod(angle + 2 * Pi / 3, 2 * Pi), 1, 2/Pi * atan(sqrt(rSq)));
    vec3 col = HSL_to_RGB(hsl);
    
   // float mag = 1.3f*length(point)/(length(point) + 1.0f);
    
    return col;
}


vec2 mult(vec2 a, vec2 b){
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 div(vec2 a, vec2 b){
    return vec2(a.x*b.x + a.y*b.y, a.x*b.y - a.y*b.x)/(b.x*b.x + b.y*b.y);
}

vec2 Cexp(vec2 z){
    float R = exp(z.x);
    vec2 v = vec2(cos(z.y), sin(z.y));
    
    return R*v;
}

vec2 Ccos(vec2 z){
    return 0.5f * (Cexp(z) + Cexp(-z));
}

vec2 Csin(vec2 z){
    return -mult(I, 0.5f * (Cexp(z) + Cexp(-z)));
}

vec2 conj(vec2 z){
    return vec2(z.x, -z.y);
}

vec2 mandelbrot_function(vec2 z, vec2 c){
    return mult(z, z) + c;
}

vec4 mandelbrot(){
    vec2 z_approx = z;
    vec2 z_centre = centre;
    vec2 z_small = z - centre;
    vec2 z_current = z;
    

    float i = 0.0f;
    int i_approx = 0;
    int i_current = 0;
    for(i = 0; i < 100; i++){
        vec2 new_z_centre = mandelbrot_function(z_centre, centre);
        vec2 new_z_small = mandelbrot_function(z_small, z - centre);

        vec2 k = z_approx - z_centre - z_small;

        //z_approx = new_z_centre + new_z_small + mult(k,k) + 2 * (mult(z_small, z_centre) + mult(k, (z_centre + z_small)));
        //z_approx = new_z_centre + mult(k,k) + 2 * mult(k, z_centre);

        z_approx = mult(z_approx, z_approx) + z;
        

        z_centre = new_z_centre;
        z_small = new_z_small;

        z_current = mandelbrot_function(z_current, z);

        if(length(z_approx) < 2.0f){
            i_approx++;
        }
        if(length(z_current) < 2.0f){
            i_current++;
        } 
        
        
    }


    col = vec4(0.55f) * i_approx/100.0f + vec4(0.45f ,0,0,1)* i_current/100.0f;
    return col;
}

vec2 iterFunction(vec2 z, vec2 c){
    vec2 z_ = mult(z,z);
    z_ = mult(mult(z_, z_), z);

    return div(c, I + z_);
}

vec2 map(vec2 z, vec2 c){
    float i = 0;
    vec2 z_c = z;

    for(i = 0; i < 100; i++){
        z_c = iterFunction(z_c, c);
    }

    return z_c;
}

void main(){

    vec2 z_ = map(z, lastMouseClick);
    vec4 col1 = vec4(getColor(z_), 1);

    z_ = map(vec2(0), z);
    vec4 col2 = vec4(getColor(z_), 1);

    col = col1*(1 - 0.5*clicked) + 0.5*clicked*col2;

    if(length(lastMouseClick - z) < 0.01/zoom)
        col = vec4(1, 1, 1, 1);
    
}