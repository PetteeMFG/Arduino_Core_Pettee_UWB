/* Alternate pin name */
PA_6_ALT1 = PA_6 | ALT1,
PA_7_ALT1 = PA_7 | ALT1,
PA_7_ALT2 = PA_7 | ALT2,
PA_7_ALT3 = PA_7 | ALT3,
PB_1_ALT1 = PB_1 | ALT1,
PB_1_ALT2 = PB_1 | ALT2,

/* SYS_WKUP */
#ifdef PWR_WAKEUP_PIN1
  SYS_WKUP1 = PA_0,
#endif
#ifdef PWR_WAKEUP_PIN2
  SYS_WKUP2 = NC,
#endif
#ifdef PWR_WAKEUP_PIN3
  SYS_WKUP3 = NC,
#endif
#ifdef PWR_WAKEUP_PIN4
  SYS_WKUP4 = NC,
#endif
#ifdef PWR_WAKEUP_PIN5
  SYS_WKUP5 = NC,
#endif
#ifdef PWR_WAKEUP_PIN6
  SYS_WKUP6 = NC,
#endif
#ifdef PWR_WAKEUP_PIN7
  SYS_WKUP7 = NC,
#endif
#ifdef PWR_WAKEUP_PIN8
  SYS_WKUP8 = NC,
#endif

/* No USB */
