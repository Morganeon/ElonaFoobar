# TODO: autogenerate config menu from this structure

config {
    screen = {
        type = "section"
        options = {
            fullscreen = {
                type = "enum"
                preload = true
                variants = ["windowed", "fullscreen", "desktop_fullscreen"]
                default = "windowed"
            }

            display_mode = {
                type = "runtime_enum"
                preload = true
            }

            music = {
                type = "enum"
                variants = ["none", "direct_music", "mci"]
                default = "direct_music"
                preload = true
            }
            sound = {
                default = true
                preload = true
            }
            high_quality_shadows = true
            object_shadow = true
            heartbeat = true
            skip_random_event_popups = true
        }

        net = {
            type = "section"
            options = {
                enabled = false
                wish = false
                chat = false
                server_list = false
            }
        }

        anime = {
            type = "section"
            options = {
                scroll = true
                always_center = true
                scroll_when_run = true

                general_wait = {
                    default = 30
                    min = 0
                    max = 50
                }

                anime_wait = {
                    default = 20
                    min = 0
                    max = 20
                }

                alert_wait = {
                    default = 50
                    min = 0
                    max = 50
                }

                auto_turn_speed = {
                    type = "enum"
                    default = "normal"
                    variants = ["normal", "high", "highest"]
                }

                attack_anime = true
                env_effect = true

                title_effect = {
                    type = "enum"
                    default = "unsupported"
                    variants = ["unsupported"]
                }

                window_anime = false

                screen_refresh = {
                    default = 2
                    min = 0
                    max = 15
                }
            }
        }


        font = {
            type = "section"
            options = {
                japanese = "Kochi Gothic.ttf"
                english = "Bitstream Sans Vera Mono.ttf"

                vertical_offset = -1 # fontVfix1
                size_adjustment = 1 # fontSfix1
            }
        }

        game = {
            type = "section"
            options = {
                default_save = {
                    type = "runtime_enum"
                    preload = true
                }

                attack_neutral_npcs = false
                story = true
                extra_help = true
                show_autoidentify = true
                show_shop_updates = true
            }
        }

        debug = {
            type = "section"
            visible = false
            options = {
                wizard = {
                    default = false
                    preload = true
                }
                noa_debug = false
            }
        }

        message = {
            type = "section"
            options = {
                add_timestamps = false

                transparency = {
                    default = 4
                    min = 0
                    max = 10
                }
            }
        }

        input = {
            type = "section"
            options = {
                joypad = {
                    default = false
                    preload = true
                }
                autodisable_numlock = true

                walk_wait = {
                    default = 5
                    min = 0
                    max = 10
                }

                start_run_wait = {
                    default = 2
                    min = 0
                    max = 5
                }

                run_wait = {
                    default = 2
                    min = 0
                    max = 10
                }

                attack_wait = {
                    default = 4
                    min = 0
                    max = 10
                }

                key_wait = {
                    default = 5
                    min = 1
                    max = 10
                }
            }
        }

        balance = {
            type = "section"
            restock_interval = {
                default = 3
                min = 0
                max = 10
            }

            extra_race = {
                default = false
                preload = true
            }

            extra_class = {
                default = false
                preload = true
            }
        }

        foobar = {
            type = "section"
            options = {
                hp_bar_position = {
                    type = "enum"
                    default = "right"
                    variants = ["hide", "left", "right"]
                }

                leash_icon = true
                autopick = true
                autosave = true
                damage_popup = true

                startup_script = {
                    default = ""
                    visible = false
                }
            }
        }

        language = {
            type = "section"
            options = {
                language = {
                    type = "runtime_enum"
                    preload = true
                }
            }
        }

        ui = {
            type = "section"
            visible = false
            doc = <<DOC
These settings are for internal use.
Changing them may break things.
DOC
            options = {
                msg_line = {
                    default = 4
                    preload = true
                }
                tile_size = {
                    default = 48
                    preload = true
                }
                font_size = {
                    default = 14
                    preload = true
                }
                inf_ver_type = {
                    default = 1
                    min = 1
                    max = 1
                    preload = true
                }
                window_x = {
                    default = 800
                    min = 800
                    max = 1600
                    preload = true
                }
                window_y = {
                    default = 800
                    min = 800
                    max = 1600
                    preload = true
                }
                clock_x = {
                    default = 0
                    preload = true
                }
                clock_w = {
                    default = 120
                    preload = true
                }
                clock_h = {
                    default = 120
                    preload = true
                }
            }
        }

        key = {
            type = "section"
            visible = false
            options = {
                north = "8 "
                south = "2 "
                west = "4 "
                east = "6 "
                northwest = "7 "
                northeast = "9 "
                southwest = "1 "
                southeast = "3 "
                wait = "5 "

                cancel = "\\"
                esc = "^"
                alter = "["
                pageup = "+"
                pagedown = "-"
                mode = "z"
                mode2 = "*"

                quick_menu = "z"
                zap = "Z"
                inventory = "X"
                quick_inventory = "x" # quickinv
                get = "g"
                get2 = "0 "
                drop = "d"
                chara_info = "c" # charainfo
                enter = " "
                eat = "e"
                wear = "w"
                cast = "v"
                drink = "q"
                read = "r"
                fire = "f"
                go_down = ">" # goDown
                go_up = "<" # goUp
                save = "S"
                search = "s"
                interact = "i"
                identify = "x"
                skill = "a"
                close = "C"
                rest = "R"
                target = "*"
                dig = "D"
                use = "t"
                bash = "b"
                open = "o"
                dip = "B"
                pray = "p"
                offer = "O"
                journal = "j"
                material = "m"
                trait = "F"
                look = "l"
                give = "G"
                throw = "T"
                ammo = "A"
                autodig = "H"

                quicksave = "F1"
                quickload = "F2"
                help = "?"
                message_log = "/"

                key_set = ["a", "b", "c", "d", "e",
                    "f", "g", "h", "i", "j",
                    "k", "l", "m", "n", "o",
                    "p", "q", "r", "s"]
            }
        }
    }
}
