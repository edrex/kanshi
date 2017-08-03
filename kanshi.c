#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

static void display_handle_mode(void *data, struct wl_output *output, uint32_t flags, int32_t width, int32_t height, int32_t refresh) {
	(void) data;
	printf("Got mode %dx%d:0x%X @ %dmHz for output %p\n", width, height, flags, refresh, output);
}

static void display_handle_geometry(void *data, struct wl_output *output, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height,
		int32_t subpixel, const char *make, const char *model, int32_t transform) {
	(void) data;
	printf("Got geometry %d,%d physical=%dx%dmm subpixel=%d make=%s model=%s transform=0x%X for output %p\n", x, y, physical_width, physical_height,
		subpixel, make, model, transform, output);
}

static void display_handle_done(void *data, struct wl_output *output) {
	(void) data;
	printf("Got done for output %p\n", output);
}

static void display_handle_scale(void *data, struct wl_output *output, int32_t factor) {
	(void) data;
	printf("Got scale %d for output %p\n", factor, output);
}

static const struct wl_output_listener output_listener = {
	.mode = display_handle_mode,
	.geometry = display_handle_geometry,
	.done = display_handle_done,
	.scale = display_handle_scale
};

struct wl_display *display = NULL;

static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {
	(void) data;
	(void) version;

	if (strcmp(interface, "wl_output") == 0) {
		struct wl_output *output = wl_registry_bind(registry, id, &wl_output_interface, 1);
		wl_output_add_listener(output, &output_listener, NULL);
	}
}

static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
	(void) data;
	(void) registry;
	printf("Got a registry losing event for %d\n", id);
}

static const struct wl_registry_listener registry_listener = {
	global_registry_handler,
	global_registry_remover,
};

int main(int argc, char **argv) {
	(void) argc;
	(void) argv;

	display = wl_display_connect(NULL);
	if (display == NULL) {
		fprintf(stderr, "Can't connect to display\n");
		exit(1);
	}
	printf("Connected to display\n");

	struct wl_registry *registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, NULL);

	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	wl_display_disconnect(display);
	printf("Disconnected from display\n");

	exit(0);
}
