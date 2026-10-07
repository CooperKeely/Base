
static inline OS_Handle os_handle_from_wl_display(struct wl_display* display){
	OS_Handle handle = {0};
	handle.u64[0] = (U64)display;
	return handle;
}

static inline struct wl_display* os_handle_to_wl_display(OS_Handle handle){
	return (struct wl_display*)handle.u64[0];
}

static void registry_handle_global(void* data, struct wl_registry *registry, 
		U32 name, const char* interface, U32 version){
	str8_printf(stdout,"interface : '%s', version: %d, name: %d\n",
			interface, version, name);
}

static void registry_handle_global_remove(void* data, struct wl_registry* registry, U32 name){

}

static const struct wl_registry_listener registry_listener = {
	.global = registry_handle_global,
	.global_remove = registry_handle_global_remove,
};



void os_gfx_init_platform(Arena* arena, OS_GFX_Context* ctx){
	struct wl_display *display = wl_display_connect(NULL);
	if(display){
		str8_printf(stdout, "Connected to display\n");
	}else{
		str8_printf(stdout, "Failed to connect to display\n");
	}

	struct wl_registry * registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, NULL);

	wl_display_roundtrip(display);


	ctx->platform_window_handle = os_handle_from_wl_display(display);

}

void os_gfx_close_platform(OS_GFX_Context* ctx){
	struct wl_display* display = os_handle_to_wl_display(ctx->platform_window_handle);
	wl_display_disconnect(display);
}




