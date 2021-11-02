declare module 'input' {
    export type EventState = {
        mouse_x: number,
        mouse_y: number,

        left: boolean,
        middle: boolean,
        right: boolean,

        scroll_x: number,
        scroll_y: number,

        // Refer to ./keys.json/KEY for mappings to key names
        keys: {[key: number]: boolean};
    };

    // The callback function is called for every event which occurs.
    // The eventType corresponds to a LibInput event type. It's mapping can be found in ./keys.json/LIBINPUT_EVENTS.
    // The state simply holds a group of values regarding to the current state of the input stream. Use it wisely.
    export function init(callback: (eventType: number, state: EventState))): void;
}

export namespace input {}
